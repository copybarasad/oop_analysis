#include "buildingmanager.h"
#include "field.h"
#include "enemymanager.h"
#include "player.h"
#include "game_tower_action_context.h"
#include <iostream>
#include <random>
#include <algorithm>

void BuildingManager::spawnBuildings(Field& field, Player& player, int count,
                                     int buildingHealth, int towerHealth,
                                     int towerRange, int towerDamage) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(1, field.getWidth() - 2);
    std::uniform_int_distribution<> disY(1, field.getHeight() - 2);
    std::uniform_int_distribution<> disInterval(3, 8);
    std::uniform_int_distribution<> disTower(0, 3);
    
    int playerX = player.getX();
    int playerY = player.getY();
    
    for (int i = 0; i < count; ++i) {
        int x, y;
        do {
            x = disX(gen);
            y = disY(gen);
        } while (!field.canPlaceEntity(x, y, playerX, playerY));
        
        if (disTower(gen) == 0) {
            towers.push_back(EnemyTower(disInterval(gen), x, y, towerRange, towerDamage, towerHealth));
        } else {
            buildings.push_back(EnemyBuilding(disInterval(gen), x, y, buildingHealth));
        }
    }
}

void BuildingManager::setEnemySpawnCallback(std::function<void(int, int, int, int)> callback) {
	enemySpawnCallback = std::move(callback);
}

void BuildingManager::updateBuildings(Field& field, EnemyManager& enemyManager, int enemyHealth, int enemyDamage) {
    for (auto& building : buildings) {
        if (building.isDestroyed()) continue;
        
        building.update();
        
        if (building.shouldSpawnEnemy()) {
            const std::vector<std::pair<int, int>> directions = {
                {0, 1}, {1, 0}, {0, -1}, {-1, 0},
                {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
            };
            
            std::vector<std::pair<int, int>> possibleSpawnPositions;
            
            int buildingX = building.getX();
            int buildingY = building.getY();
            
            for (const auto& dir : directions) {
                int newX = buildingX + dir.first;
                int newY = buildingY + dir.second;
                
                if (field.isValidPosition(newX, newY) && 
                    field.isCellPassable(newX, newY) && 
                    !enemyManager.isCellOccupiedByEnemy(newX, newY) && 
                    !isCellOccupiedByBuilding(newX, newY)) {
                    possibleSpawnPositions.emplace_back(newX, newY);
                }
            }
            
            if (!possibleSpawnPositions.empty()) {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(0, possibleSpawnPositions.size() - 1);
                
                int randomIndex = dis(gen);
                int spawnX = possibleSpawnPositions[randomIndex].first;
                int spawnY = possibleSpawnPositions[randomIndex].second;
                
                enemyManager.getEnemies().push_back(Enemy(enemyHealth, enemyDamage, spawnX, spawnY));
                building.resetSpawnTimer();
                
                // Сущность только информирует о событии через callback, не вызывая напрямую логирование
                if (enemySpawnCallback) {
                    enemySpawnCallback(buildingX, buildingY, spawnX, spawnY);
                }
            } else {
                building.resetSpawnTimer();
            }
        }
    }
    
    removeDestroyedBuildings();
}

void BuildingManager::updateTowers(Game& game) {
	GameTowerActionContext towerContext(game);
    for (auto& tower : towers) {
        if (!tower.isDestroyed()) {
            tower.update();
            tower.attackPlayer(towerContext);
        }
    }
    
    removeDestroyedBuildings();
}

bool BuildingManager::isCellOccupiedByBuilding(int x, int y) const {
    for (const auto& building : buildings) {
        if (!building.isDestroyed() && building.getX() == x && building.getY() == y) {
            return true;
        }
    }
    for (const auto& tower : towers) {
        if (!tower.isDestroyed() && tower.getX() == x && tower.getY() == y) {
            return true;
        }
    }
    return false;
}

void BuildingManager::damageBuildingAt(int x, int y, int damage) {
    for (auto& building : buildings) {
        if (!building.isDestroyed() && building.getX() == x && building.getY() == y) {
            building.takeDamage(damage);
            std::cout << "Building takes " << damage << " damage! HP: " << building.getHealth() << "/" << building.getMaxHealth() << "\n";
            if (building.isDestroyed()) {
                std::cout << "Building destroyed!\n";
            }
            return;
        }
    }
}

void BuildingManager::damageTowerAt(int x, int y, int damage) {
    for (auto& tower : towers) {
        if (!tower.isDestroyed() && tower.getX() == x && tower.getY() == y) {
            tower.takeDamage(damage);
            std::cout << "Tower takes " << damage << " damage! HP: " << tower.getHealth() << "/" << tower.getMaxHealth() << "\n";
            if (tower.isDestroyed()) {
                std::cout << "Tower destroyed!\n";
            }
            return;
        }
    }
}

void BuildingManager::removeDestroyedBuildings() {
    buildings.erase(std::remove_if(buildings.begin(), buildings.end(),
                    [](const EnemyBuilding& building) { return building.isDestroyed(); }),
                    buildings.end());
    
    towers.erase(std::remove_if(towers.begin(), towers.end(),
                 [](const EnemyTower& tower) { return tower.isDestroyed(); }),
                 towers.end());
}