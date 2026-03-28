#include "GameController.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>

#define HEALTH_PLAYER 100
#define MELEE_DAMAGE_PLAYER 20
#define RANGED_DAMAGE_PLAYER 15
#define HEALTH_ENEMY 20
#define DAMAGE_ENEMY 2
#define INTERVAL_1 7

GameController::GameController(int fieldWidth, int fieldHeight)
    : field(fieldWidth, fieldHeight),
      player(Position(0, 0), HEALTH_PLAYER, MELEE_DAMAGE_PLAYER, RANGED_DAMAGE_PLAYER),
      hand(5),
      gameRunning(true),
      movementSystem(field),
      enemyAISystem(field, player, movementSystem),
      buildingManager(buildings, towers),
      allyManager(field),
      renderSystem(field, player, enemies, buildings, traps, allies, towers),
      playerActionSystem(player, field, enemies, traps, allies, towers, buildings,
                        movementSystem, combatSystem, enemyAISystem, 
                        buildingManager, allyManager,
                        *this)
{
    std::srand(std::time(nullptr));
    notifyGameLoaded();
}

void GameController::processPlayerMove(const Position &direction) {
    playerActionSystem.processMove(direction);
    if (!player.isAlive()) {
        gameRunning = false;
        std::cout << "Game Over! Player defeated.\n";
    }
}

void GameController::processPlayerAttack(const Position &direction) {
    playerActionSystem.processAttack(direction);
}

void GameController::processPlayerAreaAttack() {
    playerActionSystem.processAreaAttack();
}

void GameController::processPlayerSwitchCombat() {
    playerActionSystem.processSwitchCombat();
}

void GameController::renderGame() const {
    renderSystem.renderGame();
}

bool GameController::useSpell(int handIndex, const Position &target) {
    if (handIndex < 0 || handIndex >= hand.size()) { 
        std::cout << "Invalid hand index: " << handIndex << ", hand size: " << hand.size() << "\n"; 
        return false; 
    }
    
    std::cout << "Using spell at index " << handIndex << " on position (" 
              << target.x << "," << target.y << ")" << std::endl;
    
    auto spell = hand.extractAt(handIndex);
    if (!spell) {
        std::cout << "Failed to extract spell at index " << handIndex << std::endl;
        return false;
    }
    
    bool used = SpellSystem::castSpell(*spell, *this, player.getPosition(), target);
    if (!used) {
        std::cout << "Spell failed to apply, returning to hand" << std::endl;
        hand.addSpell(std::move(spell));
        return false;
    }
    
    std::cout << "Spell used successfully" << std::endl;
    return true;
}

bool GameController::buyRandomSpell(int cost) {
    auto spell = SpellFactory::createRandomSpell();
    if (hand.buySpell(std::move(spell), player, cost)) {
        std::cout << "Bought a spell and added to hand.\n";
        return true;
    }
    return false;
}

Enemy *GameController::getEnemyAtPosition(const Position &position) {
    for (auto &enemy: enemies) {
        if (enemy.isAlive() && enemy.getPosition() == position) return &enemy;
    }
    return nullptr;
}

EnemyBuilding* GameController::getBuildingAtPosition(const Position &pos) {
    return buildingManager.getBuildingAtPosition(pos);
}

EnemyTower* GameController::getTowerAtPosition(const Position &pos) {
    return buildingManager.getTowerAtPosition(pos);
}

void GameController::addTrap(const Trap &t) { 
    TrapManager::addTrap(traps, t); 
}

void GameController::removeTrapAt(const Position &pos) { 
    TrapManager::removeTrapAt(traps, pos); 
}

void GameController::addAlly(const Ally &a) { 
    allyManager.addAlly(allies, a); 
}

void GameController::addTower(const EnemyTower &t) { 
    buildingManager.addTower(t); 
}

bool GameController::upgradeSpell(int spellIndex) {
    return SpellSystem::upgradeSpell(hand, spellIndex, player);
}

void GameController::prepareForNewLevel() {
    Player& player = getPlayerMutable();
    player.restoreHealth();
    player.addCoins(50);
    player.addScore(100);
    std::cout << "Player prepared for new level! Health restored." << std::endl;
}

bool GameController::isGameRunning() const { 
    return gameRunning && player.isAlive(); 
}

void GameController::removeDeadEnemies() {
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                  [](const Enemy &e){ return !e.isAlive(); }), enemies.end());
}

void GameController::addObserver(std::shared_ptr<IGameObserver> observer) {
    observers.push_back(observer);
}

void GameController::removeObserver(std::shared_ptr<IGameObserver> observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void GameController::notifyPlayerMoved(const Position& from, const Position& to) {
    for (auto& observer : observers) {
        observer->onPlayerMoved(from, to);
    }
}

void GameController::notifyPlayerAttacked(const std::string& target, int damage) {
    for (auto& observer : observers) {
        observer->onPlayerAttacked(target, damage);
    }
}

void GameController::notifyPlayerDamaged(int damage, const std::string& source) {
    for (auto& observer : observers) {
        observer->onPlayerDamaged(damage, source);
    }
}

void GameController::notifyEnemyKilled(const Position& position, int coinsReward) {
    for (auto& observer : observers) {
        observer->onEnemyKilled(position, coinsReward);
    }
}

void GameController::notifyTowerDestroyed(const Position& position) {
    for (auto& observer : observers) {
        observer->onTowerDestroyed(position);
    }
}

void GameController::notifySpellUsed(const std::string& spellName, const Position& target) {
    for (auto& observer : observers) {
        observer->onSpellUsed(spellName, target);
    }
}

void GameController::notifyCommandExecuted(const std::string& command) {
    for (auto& observer : observers) {
        observer->onCommandExecuted(command);
    }
}

void GameController::notifyLevelUp(int newLevel) {
    for (auto& observer : observers) {
        observer->onLevelUp(newLevel);
    }
}

void GameController::notifyGameSaved() {
    for (auto& observer : observers) {
        observer->onGameSaved();
    }
}

void GameController::notifyGameLoaded() {
    for (auto& observer : observers) {
        observer->onGameLoaded();
    }
}

void GameController::notifyGameStarted() {
    for (auto& observer : observers) {
        observer->onGameStarted();
    }
}

void GameController::notifyGameEnded() {
    for (auto& observer : observers) {
        observer->onGameEnded();
    }
}