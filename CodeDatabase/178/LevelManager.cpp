#include "LevelManager.h"
#include "CharacterFactory.h"
#include <iostream>
#include <random>

LevelManager::LevelManager() 
    : currentLevel(1), levelCompleted(false), gameCompleted(false),
      healthUpgraded(false), meleeUpgraded(false), rangedUpgraded(false) {}

void LevelManager::initializeLevel(GameController& controller) {
    LevelConfig config = GameConfig::getLevelConfig(currentLevel);
    setupLevel(controller, config);
    levelCompleted = false;
    
    std::cout << "\n=== LEVEL " << currentLevel << " ===" << std::endl;
    std::cout << "Field size: " << config.fieldWidth << "x" << config.fieldHeight << std::endl;
    std::cout << "Defeat enemies to complete the level!" << std::endl;
}

void LevelManager::setupLevel(GameController& controller, const LevelConfig& config) {
    Player& player = controller.getPlayerMutable();
    int upgradedHealth = config.playerHealth + (healthUpgraded ? 20 : 0);
    int upgradedMeleeDamage = config.playerMeleeDamage + (meleeUpgraded ? 5 : 0);
    int upgradedRangedDamage = config.playerRangedDamage + (rangedUpgraded ? 3 : 0);
    
    player.setMaxHealth(upgradedHealth);
    player.restoreHealth();
    player.setMeleeDamage(upgradedMeleeDamage);
    player.setRangedDamage(upgradedRangedDamage);
    player.resetScore();
    
    healthUpgraded = meleeUpgraded = rangedUpgraded = false;
    
    controller.getEnemies().clear();
    controller.getBuildings().clear();
    controller.getTowers().clear();
    controller.getTraps().clear();
    controller.getAllies().clear();
    
    for (int i = 0; i < config.enemyCount; i++) {
        Position enemyPos(5 + i * 2, 5 + i * 2);
        if (controller.getField().isValidPosition(enemyPos)) {
            auto enemy = CharacterFactory::createEnemy(enemyPos, config.enemyHealth, config.enemyDamage);
            controller.getEnemies().push_back(*enemy);
        }
    }
    
    for (int i = 0; i < config.buildingCount; i++) {
        Position buildingPos(8 + i * 3, 8 + i * 3);
        if (controller.getField().isValidPosition(buildingPos)) {
            auto building = BuildingFactory::createEnemyBuilding(buildingPos, config.buildingSpawnInterval);
            controller.getBuildings().push_back(*building);
        }
    }
    
    for (int i = 0; i < config.towerCount; i++) {
        Position towerPos(12 + i * 4, 12 + i * 4);
        if (controller.getField().isValidPosition(towerPos)) {
            auto tower = BuildingFactory::createEnemyTower(towerPos, config.towerHealth, config.towerDamage, 3);
            controller.getTowers().push_back(*tower);
        }
    }  
    controller.prepareForNewLevel();
    std::cout << "=== LEVEL " << currentLevel << " ===" << std::endl;
    std::cout << "Enemy HP: " << config.enemyHealth << " | Tower HP: " << config.towerHealth << std::endl;
    std::cout << "Enemy Damage: " << config.enemyDamage << " | Tower Damage: " << config.towerDamage << std::endl;
    std::cout << "Building Spawn Interval: " << config.buildingSpawnInterval << " turns" << std::endl;
}

void LevelManager::checkLevelCompletion(GameController& controller) {
    const Player& player = controller.getPlayer();
    
    int scoreThreshold = currentLevel * 200;
    int coinsThreshold = currentLevel * 100;
    
    if (player.getScore() >= scoreThreshold || player.getCoins() >= coinsThreshold) {
        levelCompleted = true;
        std::cout << "\n*** LEVEL " << currentLevel << " COMPLETED! ***" << std::endl;
        std::cout << "Score: " << player.getScore() << " | Coins: " << player.getCoins() << std::endl;
        controller.getPlayerMutable().addCoins(100);
        controller.getPlayerMutable().addScore(500);
        
        if (currentLevel >= GameConfig::getMaxLevels()) {
            gameCompleted = true;
            std::cout << "*** CONGRATULATIONS! YOU COMPLETED THE GAME! ***" << std::endl;
        }
    }
}

bool LevelManager::areAllEnemiesDefeated(const GameController& controller) const {
    return false;
}

bool LevelManager::areAllTowersDestroyed(const GameController& controller) const {
    return false;
}

void LevelManager::proceedToNextLevel(GameController& controller) {
    if (currentLevel < GameConfig::getMaxLevels()) {
        currentLevel++;
        std::cout << "Advancing to Level " << currentLevel << "!" << std::endl;
    } else {
        gameCompleted = true;
    }
}