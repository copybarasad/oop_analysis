#include "level_manager.h"
#include <cstdlib>
#include <iostream>

void LevelManager::setupLevel(GameState& gameState, int level) {
    int width = gameState.getField().getWidth();
    int height = gameState.getField().getHeight();

    gameState.getEnemies().clear();

    int enemyCount = calculateEnemyCount(level);
    auto& enemies = gameState.getEnemies();
    auto& player = gameState.getPlayer();
    auto& field = gameState.getField();
    auto& building = gameState.getEnemyBuilding();
    auto& tower = gameState.getEnemyTower();
    
    for (int i = 0; i < enemyCount; ++i) {
        int x, y;
        int enemyAttempts = 0;
        
        do {
            x = rand() % field.getWidth();
            y = rand() % field.getHeight();
            enemyAttempts++;
            
            if (enemyAttempts > 100) {
                std::cout << "Couldn't find a free position for enemy!" << std::endl;
                break;
            }
            
        } while ((x == player.getPositionX() && y == player.getPositionY()) || !field.isPassable(x, y) || (x == building.getPositionX() && y == building.getPositionY()) ||(x == tower.getPositionX() && y == tower.getPositionY()));
        
        Enemy enemy(x, y);
        enemy.setHealth(calculateEnemyHealth(level));
        enemies.push_back(enemy);
    }
    
    int buildingX, buildingY;
    int buildingAttempts = 0;
        
    do {
        buildingX = rand() % field.getWidth();
        buildingY = rand() % field.getHeight();
        buildingAttempts++;
            
        if (buildingAttempts > 100) {
            std::cout << "Warning: Could not find valid position for building, using default" << std::endl;
            buildingX = 1;
            buildingY = 1;
            break;
        }

    } while (!field.isPassable(buildingX, buildingY) || (buildingX == player.getPositionX() && buildingY == player.getPositionY()) || std::abs(buildingX - player.getPositionX()) < 3 || std::abs(buildingY - player.getPositionY()) < 3);
        
    gameState.getEnemyBuilding() = EnemyBuilding(buildingX, buildingY);
    gameState.getEnemyBuilding().setHealth(calculateBuildingHealth(level));
    
    int towerX, towerY;
    int towerAttempts = 0;
        
    do {
        towerX = rand() % field.getWidth();
        towerY = rand() % field.getHeight();
        towerAttempts++;
            
        if (towerAttempts > 100) {
            std::cout << "Warning: Could not find valid position for tower, using default" << std::endl;
            towerX = field.getWidth() - 2;
            towerY = field.getHeight() - 2;
            break;
        }
            
        } while (!field.isPassable(towerX, towerY) || (towerX == player.getPositionX() && towerY == player.getPositionY()) ||(towerX == gameState.getEnemyBuilding().getPositionX() && towerY == gameState.getEnemyBuilding().getPositionY()) || std::abs(towerX - player.getPositionX()) < 3 || std::abs(towerY - player.getPositionY()) < 3);
        
        gameState.getEnemyTower() = EnemyTower(towerX, towerY);
        gameState.getEnemyTower().setHealth(calculateTowerHealth(level));
    
    
    std::cout << "=== LEVEL " << level << " ===" << std::endl;
    std::cout << "Field size: " << width << "x" << height << std::endl;
    std::cout << "Enemies: " << enemyCount << std::endl;
    std::cout << "Enemie's HP is " << enemies[0].getHealth() << std::endl;
    std::cout << "Enemy building spawned at (" << buildingX << ", " << buildingY << ") with " << gameState.getEnemyBuilding().getHealth() << " HP" << std::endl;
    std::cout << "Enemy tower spawned at (" << towerX << ", " << towerY << ") with " << gameState.getEnemyTower().getHealth() << " HP" << std::endl;
    std::cout << "Tower attack range: " << gameState.getEnemyTower().getAttackRange() << std::endl;
}

int LevelManager::calculateLevelWidth(int level) {
    return 10 + level * 2;
}

int LevelManager::calculateEnemyCount(int level) {
    return 2 + level;
}

int LevelManager::calculateLevelHeight(int level) {
    return 10 + level * 2;
}

int LevelManager::calculateEnemyHealth(int level) {
    return 2 + level;
}

int LevelManager::calculateEnemyDamage(int level) {
    return 1 + level / 2;
}

int LevelManager::calculateBuildingHealth(int level) {
    return 5 + level * 2;
}

int LevelManager::calculateTowerHealth(int level) {
    return 6 + level * 2;
}