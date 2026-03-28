#include "building_manager.h"
#include "enemy_manager.h"
#include "player.h" 
#include <random>

BuildingManager::BuildingManager(GameField& field) : field(field) {}

const std::vector<EnemyBuilding>& BuildingManager::getBuildings() const {
    return buildings;
}

void BuildingManager::addBuilding(const EnemyBuilding& building) {
    buildings.push_back(building);
    field.setCellOccupied(building.getPosition(), true);
}

void BuildingManager::processBuildingTurns() {
    for (auto& building : buildings) {
        building.update();
    }
}

void BuildingManager::spawnEnemiesFromBuildings(EnemyManager& enemyManager, const Player& player) {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    for (auto& building : buildings) {
        if (building.canSpawnEnemy()) {
            Position buildingPos = building.getPosition();
            Position directions[] = {
                Position(0, -1), Position(0, 1), Position(-1, 0), Position(1, 0)
            };
            
            bool spawned = false;
            for (int i = 0; i < 4 && !spawned; ++i) {
                Position spawnPos = buildingPos + directions[i];
                if (field.isPositionPassable(spawnPos) && 
                    !field.getCell(spawnPos).getIsOccupied() && 
                    spawnPos != player.getPosition()) { 
                    
                    enemyManager.spawnEnemyAtPosition(spawnPos);
                    building.resetCooldown();
                    spawned = true;
                    break;
                }
            }
        }
    }
}

void BuildingManager::clearBuildings() {
    for (auto& building : buildings) {
        field.setCellOccupied(building.getPosition(), false);
    }
    buildings.clear();
}