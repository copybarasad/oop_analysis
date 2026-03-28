#include "game_initializer.h"
#include <random>
#include <iostream>
#include <algorithm>

GameInitializer::GameInitializer(Player& player, GameField& field,
                               EnemyManager& enemyManager, BuildingManager& buildingManager,
                               std::vector<Tower>& towers)
    : player(player), field(field), enemyManager(enemyManager), 
      buildingManager(buildingManager), towers(towers) {}

void GameInitializer::initializeGame() {
    initializeGameWithConfig(5, 2, 20, 3);
}

void GameInitializer::initializeGameWithConfig(int enemyCount, int buildingCount, 
                                             int enemyHealth, int enemyDamage) {
    enemyManager.setEnemyParams(enemyHealth, enemyDamage);
    player.setPosition(Position(0, 0));
    field.setCellOccupied(player.getPosition(), true);
    
    for (int i = 0; i < enemyCount; ++i) {
        Position enemyPos = findValidSpawnPosition();
        Enemy enemy(enemyHealth, enemyDamage);
        enemy.setPosition(enemyPos);
        enemyManager.spawnEnemy(enemy);
    }
    
    for (int i = 0; i < buildingCount; ++i) {
        Position buildingPos = findValidSpawnPosition();
        EnemyBuilding building(4 + (enemyCount / 3));
        building.setPosition(buildingPos);
        buildingManager.addBuilding(building);
    }
    
    initializeTowersForLevel(1);
    field.setCellOccupied(player.getPosition(), true);
}

void GameInitializer::initializeTowersForLevel(int level) {
    towers.clear();
    int towerCount = 1 + (level / 3);  
    towerCount = std::min(towerCount, 3);
    
    for (int i = 0; i < towerCount; i++) {
        int towerRange = 2 + (level / 4);
        towers.push_back(Tower(towerRange, 8));
    }
    
    if (!towers.empty()) {
        towers[0].setPosition(Position(1, 1));
        field.setCellOccupied(towers[0].getPosition(), true);
        
        if (towers.size() > 1) {
            towers[1].setPosition(Position(field.getWidth() - 2, field.getHeight() - 2));
            field.setCellOccupied(towers[1].getPosition(), true);
        }
        if (towers.size() > 2) {
            towers[2].setPosition(Position(field.getWidth() - 2, 1));
            field.setCellOccupied(towers[2].getPosition(), true);
        }
    }
    
    std::cout << "🔨 Initialized " << towers.size() << " towers with range " 
              << (towers.empty() ? 0 : towers[0].getRange()) << std::endl;
}

Position GameInitializer::findValidSpawnPosition() const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distX(1, field.getWidth() - 1);
    std::uniform_int_distribution<> distY(1, field.getHeight() - 1);
    
    int maxAttempts = 50;
    int attempts = 0;
    
    Position pos(distX(gen), distY(gen));
    while ((!field.isPositionPassable(pos) || field.getCell(pos).getIsOccupied()) 
           && attempts < maxAttempts) {
        pos = Position(distX(gen), distY(gen));
        attempts++;
    }
    
    if (attempts >= maxAttempts) {
        for (int y = 0; y < field.getHeight(); ++y) {
            for (int x = 0; x < field.getWidth(); ++x) {
                Position testPos(x, y);
                if (field.isPositionPassable(testPos) && !field.getCell(testPos).getIsOccupied()) {
                    return testPos;
                }
            }
        }
        return Position(1, 1);
    }
    
    return pos;
}