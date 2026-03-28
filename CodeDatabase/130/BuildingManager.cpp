#include "BuildingManager.h"
#include "EnemyManager.h"
#include "Player.h"
#include "AllyManager.h"
#include "TowerManager.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>

BuildingManager::BuildingManager() = default;

void BuildingManager::addBuilding(std::unique_ptr<EnemyBuilding> building, int x, int y) {
    if (building) {
        buildings_.push_back({std::move(building), {x, y}});
    }
}

int BuildingManager::getBuildingCount() const {
    return static_cast<int>(buildings_.size());
}

void BuildingManager::removeDeadBuildings() {
    buildings_.erase(
        std::remove_if(buildings_.begin(), buildings_.end(),
            [](const BuildingInstance& bi) {
                bool dead = !bi.building->isAlive();
                if (dead) {
                    std::cout << "Building destroyed!\n";
                }
                return dead;
            }),
        buildings_.end()
    );
}

void BuildingManager::updateBuildings(EnemyManager& enemyManager, const Player& player, AllyManager* allyManager, TowerManager* towerManager, const std::vector<std::vector<Cell>>& grid, int width, int height) {
    for (auto& building : buildings_) {
        building.building->nextTurn();
        if (building.building->canSpawn()) {
            std::vector<Pos> dirs = {{1,0}, {-1,0}, {0,1}, {0,-1}};
            for (const auto& dir : dirs) {
                int nx = building.pos.x + dir.x;
                int ny = building.pos.y + dir.y;
                if (isValidPosition(nx, ny, width, height) &&
                    grid[ny][nx].isPassable() &&
                    isCellEmpty(nx, ny, enemyManager, player, allyManager, towerManager, grid, width, height)) {
                    enemyManager.addEnemy(std::make_unique<Enemy>(40, 5), nx, ny);
                    building.building->resetTurn();
                    break;
                }
            }
        }
    }
}

bool BuildingManager::hasBuildingOnCell(int x, int y) const {
    for (const auto& building : buildings_) {
        if (building.pos.x == x && building.pos.y == y) {
            return true;
        }
    }
    return false;
}

BuildingInstance& BuildingManager::getBuildingAt(int x, int y) {
    for (auto& building : buildings_) {
        if (building.pos.x == x && building.pos.y == y) {
            return building;
        }
    }
    throw std::runtime_error("Building not found at position (" + std::to_string(x) + ", " + std::to_string(y) + ")");
}

bool BuildingManager::isValidPosition(int x, int y, int width, int height) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

bool BuildingManager::isCellEmpty(int x, int y, const EnemyManager& enemyManager, const Player& player, AllyManager* allyManager, TowerManager* towerManager, const std::vector<std::vector<Cell>>& grid, int width, int height) const {

    if (enemyManager.hasEnemyOnCell(x, y)) {
        return false;
    }


    if (hasBuildingOnCell(x, y)) {
        return false;
    }
    

    Pos playerPos = player.getPosition();
    if (playerPos.x == x && playerPos.y == y) {
        return false;
    }
    

    if (allyManager && allyManager->hasAllyOnCell(x, y)) {
        return false;
    }
    

    if (towerManager && towerManager->hasTowerOnCell(x, y)) {
        return false;
    }
    
    return true;
}
