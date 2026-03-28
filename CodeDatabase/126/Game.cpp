#include "../../headers/gamelogic/Game.h"
#include "../../headers/gamelogic/GameState.h"
#include "../../headers/managers/LevelManager.h"
#include "../../headers/managers/CombatManager.h"
#include "../../headers/managers/ObjectManager.h"
#include "../../headers/managers/SaveLoadManager.h"
#include "../../headers/field/GameField.h"
#include "../../headers/objects/Player.h"
#include "../../headers/objects/Enemy.h"
#include "../../headers/objects/EnemyTower.h"
#include "../../headers/objects/Trap.h"
#include "../../headers/commands/InputCommandProcessor.h"
#include "../../headers/render/GameRenderer.h"
#include "../../headers/gamelogic/GameManager.h"
#include "../../headers/render/VisualizationManager.h"
#include <iostream>

using namespace std;

Game::Game(int fieldWidth, int fieldHeight, int startLevel) {
    gameState = make_unique<GameState>(fieldWidth, fieldHeight, startLevel);
    levelManager = make_unique<LevelManager>(*gameState);
    combatManager = make_unique<CombatManager>(*gameState);
    objectManager = make_unique<ObjectManager>(*gameState);
    saveLoadManager = make_unique<SaveLoadManager>(*gameState);
    
    levelManager->startLevel();
}

Game::~Game() = default;

void Game::runGame() {
    GameManager<InputCommandProcessor> gameManager;
    VisualizationManager<GameRenderer> visualizationManager;
    
    while (isRunning()) {
        visualizationManager.renderGame(*this);
        gameManager.processGameTurn(*this);
        
        if (!isRunning()) break;
        
        processEnemyTurns();
        updateGameState();
        
        if (getStatus() != GameStatus::RUNNING) {
            visualizationManager.renderGameOver(getStatus(), getCurrentLevel(), getPlayer()->getScore());
            gameManager.processLevelTransition(*this, getStatus());
        }
    }
}

void Game::movePlayer(Direction dir) {
    combatManager->movePlayer(dir);
}

void Game::switchWeapon() {
    auto player = getPlayer();
    player->switchWeapon();
    cout << "Switched to " << player->getWeaponName() << " combat! Damage: " << player->getDamage() << "\n";
}

void Game::performRangedAttack(Direction dir) {
    combatManager->performRangedAttack(dir);
}

void Game::castSpell(int spellIndex, Direction dir) {
    auto player = getPlayer();
    int spellCount = player->getSpellHand().getSpellCount();
    
    if (spellIndex < 0 || spellIndex >= spellCount) {
        std::cout << "Invalid spell index!\n";
        return;
    }
    
    Position targetPos = player->getPosition().getNeighbour(dir);
    bool success = player->getSpellHand().castSpell(spellIndex, player->getPosition(), 
                                                   targetPos, gameState->getField(), 
                                                   gameState->getGameObjects());
    
    if (success) {
        gameState->updateFieldState();
    }
}

void Game::buySpell() {
    getPlayer()->buySpell();
}

void Game::saveGame() {
    try {
        saveLoadManager->saveGame("savegame.txt");
    } catch (const exception& e) {
        cout << "Save failed: " << e.what() << "\n";
    }
}

void Game::loadGame() {
    try {
        saveLoadManager->loadGame("savegame.txt");
    } catch (const exception& e) {
        cout << "Load failed: " << e.what() << "\n";
    }
}

void Game::quitGame() {
    gameState->setStatus(GameStatus::PLAYER_DIED);
    gameState->setGameRunning(false);
}

void Game::nextLevel() {
    levelManager->nextLevel();
}

void Game::restartGame() {
    int width = 15;
    int height = 15;
    int startLevel = 1;
    
    auto savedLevelManager = std::move(levelManager);
    auto savedCombatManager = std::move(combatManager);
    auto savedObjectManager = std::move(objectManager);
    auto savedSaveLoadManager = std::move(saveLoadManager);
    
    // Создаем новое состояние игры
    gameState = std::make_unique<GameState>(width, height, startLevel);
    
    levelManager = std::make_unique<LevelManager>(*gameState);
    combatManager = std::make_unique<CombatManager>(*gameState);
    objectManager = std::make_unique<ObjectManager>(*gameState);
    saveLoadManager = std::make_unique<SaveLoadManager>(*gameState);
    
    levelManager->startLevel();
}

void Game::processEnemyTurns() {
    combatManager->processEnemyTurns();
}

void Game::updateGameState() {
    objectManager->cleanupDeadObjects();
    objectManager->checkTraps();
    
    auto enemies = getEnemies();
    auto towers = getTowers();
    auto player = getPlayer();
    
    if (!player->isAlive()) {
        gameState->setStatus(GameStatus::PLAYER_DIED);
        return;
    }
    
    bool enemiesAlive = false;
    for (const auto& enemy : enemies) {
        if (enemy->isAlive()) {
            enemiesAlive = true;
            break;
        }
    }
    
    bool towersAlive = false;
    for (const auto& tower : towers) {
        if (tower->isAlive()) {
            towersAlive = true;
            break;
        }
    }
    
    if (!enemiesAlive && !towersAlive) {
        gameState->setStatus(GameStatus::LEVEL_COMPLETED);
    }
}

GameField& Game::getField() {
    return gameState->getField();
}

shared_ptr<Player> Game::getPlayer() {
    return gameState->getPlayer();
}

int Game::getCurrentLevel() const {
    return gameState->getCurrentLevel();
}

vector<shared_ptr<Enemy>> Game::getEnemies() {
    return gameState->getEnemies();
}

vector<shared_ptr<EnemyTower>> Game::getTowers() {
    return gameState->getTowers();
}

vector<shared_ptr<Trap>> Game::getTraps() {
    return gameState->getTraps();
}

GameStatus Game::getStatus() const {
    return gameState->getStatus();
}

bool Game::isRunning() const {
    return gameState->isRunning();
}