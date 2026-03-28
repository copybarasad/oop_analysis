#include "Game.h"
#include "SpellCasting.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "Renderer.h"

#include <iostream>
#include <cctype>
#include <limits>

Game::Game() 
    : field(10, 10), player(), turnCount(0), gameRunning(false), 
      saveManager("savegame.dat"), levelManager(), visualizer() {
}

void Game::display() const {
    visualizer.visualizePlayerStats(player, levelManager.getCurrentLevel());
    visualizer.visualizeSpellInfo(player);  
    visualizer.visualizeGameField(field);
}


void Game::startLevel() {
    gameRunning = true;
    visualizer.visualizeLevelStart(levelManager.getCurrentLevel());
}

void Game::enemyTurn() {
    field.moveEnemies(player);
    
    auto playerPos = field.getPlayerPosition();
    const auto& enemies = field.getEnemies();
    const auto& enemyPositions = field.getEnemyPositions();
    
    for (size_t i = 0; i < enemies.size(); ++i) {
        auto enemyPos = enemyPositions[i];
        int dx = std::abs(enemyPos.first - playerPos.first);
        int dy = std::abs(enemyPos.second - playerPos.second);
        if (dx <= 1 && dy <= 1) {
            int damage = enemies[i].getDamage();
            player.takeDamage(damage);
            visualizer.visualizeEnemyAttack(enemies[i], damage);
        }
    }
}

void Game::buildingTurn() {
    field.spawnEnemiesFromBuildings();
}

bool Game::checkWinCondition() const {
    return levelManager.shouldLevelUp(player, field);
}

bool Game::checkLoseCondition() const {
    return !player.isAlive();
}

void Game::gameOver() {
    visualizer.visualizeGameOver(player.getScore());
}

void Game::saveGame() {
    try {
        saveManager.saveGameWithLevel(field, player, turnCount, 
                             levelManager.getCurrentLevel(), 
                             levelManager.getScoreForNextLevel());
        
        visualizer.visualizeGameStateOperation("save", levelManager.getCurrentLevel());
        
    } catch (const std::exception& e) {
        visualizer.visualizeError("saving game", e.what());
    }
}

void Game::loadGame() {
    try {
        int currentLevel, scoreForNextLevel;
        saveManager.loadGameWithLevel(field, player, turnCount, currentLevel, scoreForNextLevel);
        levelManager.setLevelState(currentLevel, scoreForNextLevel);
        gameRunning = true;
        
        visualizer.visualizeGameStateOperation("load", levelManager.getCurrentLevel());
        visualizer.visualizeOperationResult("game", "loaded", 
            "Continuing from turn " + std::to_string(turnCount));
        
    } catch (const std::exception& e) {
        visualizer.visualizeError("loading game", e.what());
        throw;
    }
}

void Game::showSaveInfo() const {
    std::string saveInfo = saveManager.getSaveInfo();
    visualizer.visualizeSaveFileInfo(saveInfo);
}

void Game::play() {
    if (checkWinCondition()) {
        visualizer.visualizeOperationResult("level", "completed", 
            "Level " + std::to_string(levelManager.getCurrentLevel()) + 
            " completed! All enemies defeated!");
        nextLevel();
    }
    
    if (checkLoseCondition()) {
        gameOver();
        gameRunning = false;
    }
}

bool Game::processPlayerMove(char direction) {
    if (!player.canMoveNextTurn()) {
        visualizer.visualizeOperationResult("movement", "blocked", "Player cannot move this turn (slowed)!");
        
        if (direction == 'f' || direction == 'F') {
            player.switchCombatType();
            visualizer.visualizeOperationResult("combat", "switched", 
                "Switched to " + std::string(player.getCombatType() == CombatType::MELEE ? "melee" : "ranged") + " combat");
            return true;
        } else if (direction == 'c' || direction == 'C') {
            return true;
        } else if (direction == 'w' || direction == 'W' || direction == 's' || direction == 'S' || 
                  direction == 'a' || direction == 'A' || direction == 'd' || direction == 'D') {
            visualizer.visualizeOperationResult("movement", "blocked", "Movement blocked by slow effect!");
            player.resetMovement();
            return true;
        } else {
            visualizer.visualizeOperationResult("command", "invalid", "Invalid command!");
            return true;
        }
    }

    std::pair<int, int> playerPos = field.getPlayerPosition();
    int x = playerPos.first;
    int y = playerPos.second;
    int newX = x, newY = y;
    
    switch (direction) {
        case 'w': case 'W': newY--; break;
        case 's': case 'S': newY++; break;
        case 'a': case 'A': newX--; break;
        case 'd': case 'D': newX++; break;
        case 'f': case 'F': 
            player.switchCombatType();
            visualizer.visualizeOperationResult("combat", "switched", 
                "Switched to " + std::string(player.getCombatType() == CombatType::MELEE ? "melee" : "ranged") + " combat");
            return true;
        default:
            visualizer.visualizeError("move input", "Invalid direction");
            return false;
    }
    
    if (field.setPlayerPosition(newX, newY, player)) {
        if (field.getCellType(newX, newY) == CellType::SLOW) {
            player.setMovement(false);
            visualizer.visualizeOperationResult("movement", "slowed", "Player stepped on slow cell! Cannot move next turn.");
        }
        return true;
    } else {
        visualizer.visualizeOperationResult("movement", "blocked", "Cannot move there!");
        return false;
    }
}

void Game::processEnemyTurns() {
    int enemiesBefore = field.getEnemyCount();
    
    enemyTurn();
    buildingTurn();
    field.removeDeadEnemies();
    
    int enemiesAfter = field.getEnemyCount();
    if (enemiesAfter < enemiesBefore) {
        int enemiesKilled = enemiesBefore - enemiesAfter;
        player.restoreMana(enemiesKilled * 10);
        visualizer.visualizeOperationResult("resource", "mana_gained", 
            "Gained " + std::to_string(enemiesKilled * 10) + " mana for killing enemies!");
    }
    
    turnCount++;
    if (turnCount % 10 == 0) {
        saveGame();
    }
}

void Game::buySpell() {
    visualizer.visualizeSpellShop(player.getScore(), player.getSpellHand().getSpellCount(), player.getSpellHand().getMaxSize());
}



void Game::castSpell(int spellIndex, int targetX, int targetY) {
    bool success = SpellCasting::castSpell(player, spellIndex, field, targetX, targetY);
    if (!success) {
        visualizer.visualizeOperationResult("spell", "failed", "");
    }
}


void Game::switchCombatType() {
    visualizer.visualizeOperationResult("combat", "switched", 
        "Switched to " + std::string(player.getCombatType() == CombatType::MELEE ? "melee" : "ranged") + " combat");
}

void Game::startNewGame(int width, int height) {
    if (width < 10) width = 10;
    if (width > 20) width = 20;
    if (height < 10) height = 10;
    if (height > 20) height = 20;
    
    player = Player(100, 20, 15);
    turnCount = 0;
    field = GameField(width, height);
    levelManager.resetToLevel(1);
    
    auto starterSpell = SpellCasting::getRandomStarterSpell();
    player.getSpellHand().addSpell(std::move(starterSpell));
    
    startLevel();
}

void Game::nextLevel() {
    levelManager.advanceLevel(player, field);
    turnCount = 0;
    visualizer.visualizeLevelStart(levelManager.getCurrentLevel());
}