#include "GameController.h"
#include "GameLevel.h"      
#include "SaveManager.h"
#include "LevelGenerator.h" 
#include "ISpell.h"
#include "FireballSpell.h"
#include "ZapSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "BuffSpell.h"
#include <iostream>
#include <chrono>
#include <random>
#include <limits> 

const std::string QUICKSAVE_FILENAME = "quicksave.dat";

GameController::GameController(Player& p, Field& f, EventLogger& l)
    : player(p), 
      initialFieldRef(f),
      currentLevelNumber(0),
      exitRequested(false),
      eventLogger(l) 
{
    currentLevelObj = std::make_unique<GameLevel>(f.getWidth(), f.getHeight());
}

void GameController::logEvent(LogMsg msg) { eventLogger.log(msg); }
void GameController::logEvent(LogMsg msg, int value) { eventLogger.log(msg, value); }
void GameController::logEvent(LogMsg msg, const std::string& text) { eventLogger.log(msg, text); }

const Field& GameController::getField() const { return currentLevelObj->getField(); }
const Player& GameController::getPlayer() const { return player; }
GameLevel* GameController::getLevel() { return currentLevelObj.get(); }

void GameController::initialize() { startNextLevel(); }

void GameController::restartGame() {
    player.reset();
    currentLevelNumber = 0;
    currentLevelObj = std::make_unique<GameLevel>(15, 15);
    startNextLevel();
    logEvent(LogMsg::GAME_RESTARTED);
}

bool GameController::isExitRequested() const { return exitRequested; }
bool GameController::isPlayerAlive() const { return player.isAlive(); }
int GameController::getCurrentLevel() const { return currentLevelNumber; }


void GameController::handleExit() { 
    exitRequested = true; 
    logEvent(LogMsg::EXITING); 
}

void GameController::handleSave() {
    performSave(QUICKSAVE_FILENAME); 
}

void GameController::handleLoad() {
    performLoad(QUICKSAVE_FILENAME); 
}

void GameController::handleSwitchMode() {
    player.switchMode();
    logEvent(LogMsg::SWITCH_MODE, player.getModeString());
    processTurn();
}

void GameController::handleBuySpell() {
    const int spellCost = 30;
    if (player.getScore() < spellCost) { 
        logEvent(LogMsg::SPELL_NOT_ENOUGH_SCORE, spellCost); 
        return; 
    }
    if (player.getHand()->getSpellCount() >= 5) { 
        logEvent(LogMsg::SPELL_HAND_FULL); 
        return; 
    }

    unsigned seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> distrib(0, 4);
    
    std::unique_ptr<ISpell> randomSpell;
    switch (distrib(gen)) {
        case 0: randomSpell = std::make_unique<FireballSpell>(); break;
        case 1: randomSpell = std::make_unique<ZapSpell>(); break;
        case 2: randomSpell = std::make_unique<TrapSpell>(); break;
        case 3: randomSpell = std::make_unique<SummonSpell>(); break;
        default: randomSpell = std::make_unique<BuffSpell>(); break;
    }
    std::string spellName = randomSpell->getName();
    player.getHand()->addSpell(std::move(randomSpell));
    player.spendScore(spellCost);
    logEvent(LogMsg::SPELL_BOUGHT, spellName);
    processTurn();
}

void GameController::handleCastSpell() {
    PlayerHand* hand = player.getHand();
    if (hand->getSpellCount() == 0) {
        logEvent(LogMsg::SPELL_NO_SPELLS);
        return;
    }
    int idx = getInputInt("Choose spell (0-" + std::to_string(hand->getSpellCount()-1) + "): ");
    ISpell* spell = hand->getSpell(idx);
    if (spell) {
        if (spell->cast(player, *this)) {
            hand->removeSpell(idx);
            processTurn();
        } else {
            logEvent(LogMsg::SPELL_CAST_CANCEL);
        }
    } else {
        logEvent(LogMsg::SPELL_INVALID_INDEX);
    }
}

void GameController::handlePlayerMove(int dx, int dy) {
    if (player.isSlowedThisTurn()) {
        logEvent(LogMsg::MOVED_SLOWED);
        player.processTrap();
        processTurn();
        return;
    }
    Field& f = currentLevelObj->getField();
    
    if (player.getMode() == CombatMode::MELEE) {
        int targetX = player.getX() + dx, targetY = player.getY() + dy;
        CellType targetType = f.getCellType(targetX, targetY);
        
        if (targetType == CellType::ENEMY || targetType == CellType::ENEMY_TOWER || targetType == CellType::SPAWNER) {
            player.attack(dx, dy, f, currentLevelObj->getEnemiesEx(), currentLevelObj->getSpawnersEx(), currentLevelObj->getTowersEx());
            logEvent(LogMsg::ATTACK_MELEE);
            processTurn();
        } else {
            MoveResult result = player.move(dx, dy, f);
            if (result == MoveResult::SUCCESS) {
                processTurn();
            } else if (result == MoveResult::SUCCESS_SLOWED) { 
                logEvent(LogMsg::STEPPED_TRAP); 
                processTurn(); 
            } else if (result == MoveResult::WALL) {
                logEvent(LogMsg::HIT_WALL);
            } else if (result == MoveResult::OCCUPIED) {
                logEvent(LogMsg::CELL_OCCUPIED);
            }
        }
    } else {
        player.attack(dx, dy, f, currentLevelObj->getEnemiesEx(), currentLevelObj->getSpawnersEx(), currentLevelObj->getTowersEx());
        logEvent(LogMsg::ATTACK_RANGED);
        processTurn();
    }
}

void GameController::processTurn() {
    checkLevelCompletion();
    if (exitRequested) return;
    currentLevelObj->update(player, *this, &eventLogger);
    currentLevelObj->cleanupDead(player, &eventLogger);
}

void GameController::checkLevelCompletion() {
    if (currentLevelObj->isCleared()) {
        logEvent(LogMsg::LEVEL_COMPLETE, currentLevelNumber);
        levelUpMenu();
        startNextLevel();
    }
}

void GameController::levelUpMenu() {
    int choice = getInputInt("\n--- Level Up! ---\nChoose upgrade:\n1. Increase Health (+20)\n2. Increase Damage (+5)\n> ");
    if (choice == 1) { 
        player.upgradeHealth(20); 
        logEvent(LogMsg::UPGRADE_HEALTH); 
    } else { 
        player.upgradeDamage(5); 
        logEvent(LogMsg::UPGRADE_DAMAGE); 
    }
    player.healFull();
    int spellsToRemove = player.getHand()->getSpellCount() / 2;
    if (spellsToRemove > 0) {
        player.getHand()->removeRandomSpells(spellsToRemove);
        logEvent(LogMsg::FATIGUE_LOST_SPELLS, spellsToRemove);
    }
}

void GameController::startNextLevel() {
    currentLevelNumber++;
    currentLevelObj = std::make_unique<GameLevel>(15, 15);
    LevelGenerator::generate(currentLevelNumber, currentLevelObj->getField(), player, currentLevelObj->getEnemiesEx(), currentLevelObj->getSpawnersEx(), currentLevelObj->getTrapsEx(), currentLevelObj->getAlliesEx(), currentLevelObj->getTowersEx());
    logEvent(LogMsg::LEVEL_START, currentLevelNumber);
}

int GameController::getInputInt(const std::string& prompt) {
    std::cout << prompt;
    int val;
    bool isValid = false;
    while (!isValid) {
        if (std::cin >> val) isValid = true;
        else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. " << prompt;
        }
    }
    return val;
}

char GameController::getInputChar(const std::string& prompt) {
    std::cout << prompt;
    char val;
    std::cin >> val;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return val;
}

void GameController::damageAt(int x, int y, int damage) { currentLevelObj->damageAt(x, y, damage); }
void GameController::damagePlayer(int amount) { player.takeDamage(amount); }
void GameController::placeTrap(int x, int y, int damage) { currentLevelObj->placeTrap(x, y, damage); }
bool GameController::spawnAllyNear(int x, int y) { return currentLevelObj->spawnAllyNear(x, y); }
void GameController::addEnemy(const Enemy& enemy) { currentLevelObj->getEnemiesEx().push_back(enemy); }
void GameController::addSpawner(const EnemySpawner& spawner) { currentLevelObj->getSpawnersEx().push_back(spawner); }
void GameController::addTower(const EnemyTower& tower) { currentLevelObj->getTowersEx().push_back(tower); }
void GameController::performSave(const std::string& filename) {
    SaveManager::save(filename, currentLevelNumber, player, currentLevelObj->getField(), currentLevelObj->getEnemies(), currentLevelObj->getSpawners(), currentLevelObj->getTowers(), eventLogger);
}

void GameController::performLoad(const std::string& filename) {
    currentLevelObj->clearEntities();
    SaveManager::load(filename, currentLevelNumber, player, currentLevelObj->getField(), currentLevelObj->getEnemiesEx(), currentLevelObj->getSpawnersEx(), currentLevelObj->getTowersEx(), eventLogger);
}