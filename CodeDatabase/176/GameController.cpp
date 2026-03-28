#include "GameController.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "EnhanceSpell.h"
#include <iostream>
#include <random>

GameController::GameController()
    : gameRunning(false), currentLevel(1), playerScore(0),
      gameSave("game_save.dat") {
}

void GameController::startNewGame() {
    currentLevel = 0;
    playerScore = 0;
    
    nextLevel();
    gameRunning = true;
}

void GameController::nextLevel() {
    if (gameManager) {
        playerScore = gameManager->getPlayer().getScore();
        
        if (currentLevel >= 2) {
            auto* spellHand = const_cast<Player&>(gameManager->getPlayer()).getSpellHand();
            if (spellHand && spellHand->getSpellCount() > 0) {
                std::random_device rd;
                std::mt19937 gen(rd());
                
                int spellsToRemove = spellHand->getSpellCount() / 2;
                for (int i = 0; i < spellsToRemove; i++) {
                    if (spellHand->getSpellCount() > 0) {
                        std::uniform_int_distribution<> dis(0, spellHand->getSpellCount() - 1);
                        spellHand->removeSpell(dis(gen));
                    }
                }
                std::cout << "Half of your spells were lost during the transition!\n";
            }
        }
    }
    
    currentLevel++;
    
    int newSize = 10 + currentLevel * 2;
    if (newSize > 25) newSize = 25;
    
    gameManager = std::make_unique<GameManager>(newSize, newSize, currentLevel);
    
    Player& player = const_cast<Player&>(gameManager->getPlayer());
    player.restoreHealth();
    if (playerScore > 0) {
        player.addScore(playerScore);
    }
    
    if (currentLevel == 1) {
        initializeStartingSpell();
    }
    
    int enemyHealth = currentLevel * 10;
    int enemyDamage = currentLevel * 5;
    
    int buildingX = std::min(5, newSize - 3);
    int buildingY = std::min(5, newSize - 3);
    int towerX = std::min(8, newSize - 2);
    int towerY = std::min(8, newSize - 2);
    
    gameManager->addEnemyBuilding(buildingX, buildingY, 4, enemyHealth, enemyDamage);
    gameManager->addEnemyTower(towerX, towerY, 2);
    
    std::cout << "=== STARTING LEVEL " << currentLevel << " ===\n";
    std::cout << "Field: " << newSize << "x" << newSize << "\n";
    std::cout << "Enemies to defeat: " << currentLevel * 3 << "\n";
}

bool GameController::isLevelComplete() const {
    if (!gameManager) return false;
    int requiredKills = currentLevel * 3;
    return gameManager->getPlayer().getEnemiesKilled() >= requiredKills;
}



void GameController::initializeStartingSpell() {
    auto* spellHand = gameManager->getPlayer().getSpellHand();
    if (spellHand && spellHand->getSpellCount() == 0) {
        spellHand->addSpell(std::make_unique<DirectDamageSpell>(20, 3, 10));
        std::cout << "Starting spell: Fire Bolt in slot 1\n";
    }
}

void GameController::processPlayerAction(Direction direction) {
    if (!gameRunning) return;
    gameManager->movePlayer(direction);
}

void GameController::processAttack(Direction direction) {
    if (!gameRunning) return;
    gameManager->attackEnemy(direction);
}

void GameController::processCombatModeSwitch() {
    if (!gameRunning) return;
    gameManager->switchPlayerCombatMode();
}

void GameController::processSpellCast(int spellIndex, int targetX, int targetY) {
    if (!gameRunning) return;
    if (gameManager->castSpell(spellIndex, targetX, targetY)) {
    }
}

void GameController::processSpellPurchase() {
    if (!gameRunning) return;
    if (gameManager->purchaseSpell(30)) {
    }
}

void GameController::updateGame() {
    if (!gameRunning) return;
    
    gameManager->updateGame();

    if (gameManager->isGameOver()) {
        gameRunning = false;
        std::cout << "\n=== GAME OVER ===\n";
        const auto& player = gameManager->getPlayer();
        std::cout << "Final Score: " << player.getScore() << "\n";
        std::cout << "Enemies Killed: " << player.getEnemiesKilled() << "\n";
        std::cout << "Level Reached: " << currentLevel << "\n";
    } else if (isLevelComplete()) {
        if (currentLevel >= 4) {
            std::cout << "\n🎉 CONGRATULATIONS! YOU COMPLETED ALL 4 LEVELS! 🎉\n";
            std::cout << "Final Score: " << playerScore << "\n";
            std::cout << "Total Enemies Killed: " << gameManager->getPlayer().getEnemiesKilled() << "\n";
            gameRunning = false;
            return;
        }
        
        std::cout << "\n=== LEVEL " << currentLevel << " COMPLETED! ===\n";
        std::cout << "Preparing next level...\n";
        nextLevel();
    }
}

bool GameController::isGameRunning() const {
    return gameRunning;
}

bool GameController::isGameOver() const {
    return gameManager && gameManager->isGameOver();
}


bool GameController::saveGame() {
    return gameSave.saveGame(*this);
}

bool GameController::loadGame() {
    auto data = gameSave.loadGame();
    if (data) {
        loadFromData(*data);
        return true;
    }
    return false;
}

void GameController::loadFromData(const GameSaveData& data) {
    try {
        if (data.currentLevel < 1 || data.currentLevel > 4) {
            throw DataValidationException("Invalid level number");
        }
        
        currentLevel = data.currentLevel;
        playerScore = data.playerScore;

        gameManager = std::make_unique<GameManager>(data.fieldWidth, data.fieldHeight, currentLevel);
        gameManager->setPlayerPosition(data.playerX, data.playerY);
        
        GameField& field = const_cast<GameField&>(gameManager->getField());
        field.clearField();
        
        for (int y = 0; y < data.fieldHeight; ++y) {
            for (int x = 0; x < data.fieldWidth; ++x) {
                int index = y * data.fieldWidth + x;
                if (index < data.cellTypes.size()) {
                    CellType cellType;
                    switch (data.cellTypes[index]) {
                        case 0: cellType = CellType::EMPTY; break;
                        case 1: cellType = CellType::OBSTACLE; break;
                        case 2: cellType = CellType::SLOW; break;
                        default: throw DataValidationException("Invalid cell type");
                    }
                    field.setCellType(x, y, cellType);
                }
            }
        }
        
        for (const auto& pos : data.enemyPositions) {
            if (field.isValidPosition(pos.first, pos.second) && field.isCellPassable(pos.first, pos.second)) {
                Enemy enemy(30, currentLevel * 5);
                field.addEnemyAt(pos.first, pos.second, enemy);
            }
        }
        
        for (const auto& pos : data.trapPositions) {
            if (field.isValidPosition(pos.first, pos.second)) {
                field.placeTrap(pos.first, pos.second);
            }
        }
        
        for (const auto& pos : data.allyPositions) {
            if (field.isValidPosition(pos.first, pos.second)) {
                field.placeAlly(pos.first, pos.second);
            }
        }
        
        Player& player = const_cast<Player&>(gameManager->getPlayer());
        player.restoreHealth();
        if (data.playerHealth < player.getHealth()) {
            player.takeDamage(player.getHealth() - data.playerHealth);
        }
        player.addMana(data.playerMana - player.getMana());
        player.addScore(data.playerScore - player.getScore());
        
        for (int i = player.getEnemiesKilled(); i < data.enemiesKilled; i++) {
            player.addEnemyKilled();
        }
        
        if (data.playerCombatMode == 1) {
            player.switchCombatMode();
        }
        
        if (data.hasStartingSpell && currentLevel == 1) {
            initializeStartingSpell();
        }
        
        int enemyHealth = currentLevel * 10;
        int enemyDamage = currentLevel * 5;
        
        int buildingX = std::min(5, data.fieldWidth - 3);
        int buildingY = std::min(5, data.fieldHeight - 3);
        int towerX = std::min(8, data.fieldWidth - 2);
        int towerY = std::min(8, data.fieldHeight - 2);
        
        gameManager->addEnemyBuilding(buildingX, buildingY, 4, enemyHealth, enemyDamage);
        gameManager->addEnemyTower(towerX, towerY, 2);
        
        gameRunning = true;
        std::cout << "=== LOADED LEVEL " << currentLevel << " ===\n";
        
    } catch (const std::exception& e) {
        std::cout << "Error during game loading: " << e.what() << "\n";
        gameRunning = false;
    }
}

