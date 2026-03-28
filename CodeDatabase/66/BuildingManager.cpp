#include "../include/BuildingManager.h"
#include "../include/Enemy.h"
#include "../include/Direction.h"
#include <vector>

void BuildingManager::addBuilding(const Position& pos, std::unique_ptr<EnemyBuilding> building) {
    buildings.push_back({pos, std::move(building)});
}

bool BuildingManager::hasBuildingAt(const Position& pos) const {
    for (const auto& buildingPair : buildings) {
        if (buildingPair.first == pos) {
            return true;
        }
    }
    return false;
}

void BuildingManager::processSpawns(EnemyManager& enemyManager, const MapGrid& grid) {
    for (auto& buildingPair : buildings) {
        if (buildingPair.second->shouldSpawn()) {
            Position spawnPos = findNearbySpawnPosition(buildingPair.first, grid, enemyManager);
            
            if (spawnPos != buildingPair.first) {
                std::vector<Direction> basicPattern = {Direction::UP, Direction::RIGHT, Direction::DOWN, Direction::LEFT};
                enemyManager.addEnemy(spawnPos, std::make_unique<Enemy>(40, 10, basicPattern));
            }
            
            buildingPair.second->resetSpawnTimer();
        }
        buildingPair.second->processTurn();
    }
}

const std::vector<std::pair<Position, std::unique_ptr<EnemyBuilding>>>& BuildingManager::getBuildings() const {
    return buildings;
}

int BuildingManager::getBuildingIndexAt(const Position& pos) const {
    for (size_t i = 0; i < buildings.size(); i++) {
        if (buildings[i].first == pos) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int BuildingManager::attackBuildingAt(const Position& pos, int damage) {
    int index = getBuildingIndexAt(pos);
    if (index != -1) {
        return buildings[index].second->takeDamage(damage);
    }
    return 0;
}

int BuildingManager::removeDeadBuildings() {
    int destroyedCount = 0;
    for (auto it = buildings.begin(); it != buildings.end(); ) {
        if (!it->second->isAlive()) {
            it = buildings.erase(it);
            destroyedCount++;
        } else {
            ++it;
        }
    }
    return destroyedCount;
}

Position BuildingManager::findNearbySpawnPosition(const Position& buildingPos, const MapGrid& grid, const EnemyManager& enemyManager) const {
    std::vector<Position> candidates;
    
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue;
            
            Position candidate(buildingPos.getX() + dx, buildingPos.getY() + dy);
            if (grid.canMoveTo(candidate) && 
                !enemyManager.isPositionOccupiedByEnemy(candidate) &&
                !hasBuildingAt(candidate)) {
                candidates.push_back(candidate);
            }
        }
    }
    
    if (!candidates.empty()) {
        return candidates[0];
    }
    
    return buildingPos;
}
