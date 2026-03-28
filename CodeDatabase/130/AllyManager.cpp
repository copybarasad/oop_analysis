#include "AllyManager.h"
#include "Player.h"
#include "EnemyManager.h"
#include "Enemy.h"
#include "BuildingManager.h"
#include "TowerManager.h"
#include <algorithm>
#include <cmath>
#include <iostream>

void AllyManager::addAlly(std::unique_ptr<Ally> ally, int x, int y) {
    if (ally) {
        ally->setPosition(x, y);
        allies_.push_back(std::move(ally));
    }
}

int AllyManager::getAllyCount() const {
    return static_cast<int>(allies_.size());
}

void AllyManager::removeDeadAllies() {
    allies_.erase(
        std::remove_if(allies_.begin(), allies_.end(),
            [](const std::unique_ptr<Ally>& ally) {
                return !ally->isAlive();
            }),
        allies_.end()
    );
}

bool AllyManager::hasAllyOnCell(int x, int y) const {
    for (const auto& ally : allies_) {
        if (ally && ally->isAlive()) {
            Pos pos = ally->getPosition();
            if (pos.x == x && pos.y == y) {
                return true;
            }
        }
    }
    return false;
}

Ally& AllyManager::getAllyAt(int x, int y) const {
    for (const auto& ally : allies_) {
        if (ally && ally->isAlive()) {
            Pos pos = ally->getPosition();
            if (pos.x == x && pos.y == y) {
                return *ally.get();
            }
        }
    }
    throw std::runtime_error("Ally not found at position");
}

void AllyManager::processAllyMovement(const Player& player, EnemyManager& enemyManager,
                                     const BuildingManager& buildingManager,
                                     const TowerManager& towerManager,
                                     const std::vector<std::vector<Cell>>& grid,
                                     int width, int height) {
    const auto& enemies = enemyManager.getEnemies();
    const auto& buildings = buildingManager.getBuildings();
    const auto& towers = towerManager.getTowers();
    
    for (auto& ally : allies_) {
        if (!ally || !ally->isAlive()) continue;
        if (ally->shouldSkipTurn()) {
            ally->resetSkipTurn();
            continue;
        }
        
        Pos allyPos = ally->getPosition();


        enum class TargetType { NONE, ENEMY, BUILDING, TOWER };
        TargetType targetType = TargetType::NONE;
        Pos targetPos;
        int minDist = 9999;
        

        for (Enemy* enemy : enemies) {
            if (!enemy || !enemy->isAlive()) continue;
            Pos enemyPos = enemy->getPosition();
            int dist = std::abs(allyPos.x - enemyPos.x) + std::abs(allyPos.y - enemyPos.y);
            if (dist < minDist) {
                minDist = dist;
                targetPos = enemyPos;
                targetType = TargetType::ENEMY;
            }
        }
        

        for (const auto& buildingInst : buildings) {
            if (!buildingInst.building || !buildingInst.building->isAlive()) continue;
            int dist = std::abs(allyPos.x - buildingInst.pos.x) + std::abs(allyPos.y - buildingInst.pos.y);
            if (dist < minDist) {
                minDist = dist;
                targetPos = buildingInst.pos;
                targetType = TargetType::BUILDING;
            }
        }
        

        for (const auto& tower : towers) {
            if (!tower || !tower->isAlive()) continue;
            Pos towerPos = tower->getPosition();
            int dist = std::abs(allyPos.x - towerPos.x) + std::abs(allyPos.y - towerPos.y);
            if (dist < minDist) {
                minDist = dist;
                targetPos = towerPos;
                targetType = TargetType::TOWER;
            }
        }
        
        if (targetType == TargetType::NONE) continue;
        
        int dx = targetPos.x - allyPos.x;
        int dy = targetPos.y - allyPos.y;
        
        int nx = allyPos.x;
        int ny = allyPos.y;


        auto attackTarget = [&](int tx, int ty) -> bool {
            if (enemyManager.hasEnemyOnCell(tx, ty)) {
                Enemy& enemy = enemyManager.getEnemyAt(tx, ty);
                enemy.takeDamage(ally->getDamage());
                std::cout << "Ally at (" << allyPos.x << "," << allyPos.y << ") attacks enemy at (" 
                         << tx << "," << ty << ") for " << ally->getDamage() << " damage! Enemy HP: " 
                         << enemy.getHealth() << "/" << enemy.getMaxHealth() << "\n";
                return true;
            }
            if (buildingManager.hasBuildingOnCell(tx, ty)) {
                BuildingInstance& buildingInst = const_cast<BuildingManager&>(buildingManager).getBuildingAt(tx, ty);
                buildingInst.building->takeDamage(ally->getDamage());
                std::cout << "Ally at (" << allyPos.x << "," << allyPos.y << ") attacks building at (" 
                         << tx << "," << ty << ") for " << ally->getDamage() << " damage! Building HP: " 
                         << buildingInst.building->getHealth() << "/" << buildingInst.building->getMaxHealth() << "\n";
                return true;
            }
            if (towerManager.hasTowerOnCell(tx, ty)) {
                EnemyTower& tower = const_cast<TowerManager&>(towerManager).getTowerAt(tx, ty);
                tower.takeDamage(ally->getDamage());
                std::cout << "Ally at (" << allyPos.x << "," << allyPos.y << ") attacks tower at (" 
                         << tx << "," << ty << ") for " << ally->getDamage() << " damage! Tower HP: " 
                         << tower.getHealth() << "/" << tower.getMaxHealth() << "\n";
                return true;
            }
            return false;
        };

        if (std::abs(dx) > std::abs(dy)) {
            if (dx != 0 && isValidPosition(nx + (dx/std::abs(dx)), ny, width, height) &&
                isCellPassable(grid[ny][nx + (dx/std::abs(dx))])) {
                int tx = nx + (dx/std::abs(dx));
                int ty = ny;
                if (!attackTarget(tx, ty) && isCellEmpty(tx, ty, player, enemyManager, buildingManager, towerManager, grid, width, height)) {
                    nx = tx;
                }
            } else if (dy != 0 && isValidPosition(nx, ny + (dy/std::abs(dy)), width, height) &&
                       isCellPassable(grid[ny + (dy/std::abs(dy))][nx])) {
                int tx = nx;
                int ty = ny + (dy/std::abs(dy));
                if (!attackTarget(tx, ty) && isCellEmpty(tx, ty, player, enemyManager, buildingManager, towerManager, grid, width, height)) {
                    ny = ty;
                }
            }
        } else {
            if (dy != 0 && isValidPosition(nx, ny + (dy/std::abs(dy)), width, height) &&
                isCellPassable(grid[ny + (dy/std::abs(dy))][nx])) {
                int tx = nx;
                int ty = ny + (dy/std::abs(dy));
                if (!attackTarget(tx, ty) && isCellEmpty(tx, ty, player, enemyManager, buildingManager, towerManager, grid, width, height)) {
                    ny = ty;
                }
            } else if (dx != 0 && isValidPosition(nx + (dx/std::abs(dx)), ny, width, height) &&
                       isCellPassable(grid[ny][nx + (dx/std::abs(dx))])) {
                int tx = nx + (dx/std::abs(dx));
                int ty = ny;
                if (!attackTarget(tx, ty) && isCellEmpty(tx, ty, player, enemyManager, buildingManager, towerManager, grid, width, height)) {
                    nx = tx;
                }
            }
        }
        
        if (nx != allyPos.x || ny != allyPos.y) {
            std::cout << "Ally at (" << allyPos.x << "," << allyPos.y << ") moves to (" << nx << "," << ny << ")\n";
            ally->setPosition(nx, ny);

            if (isValidPosition(nx, ny, width, height) && grid[ny][nx].getType() == CellType::SLOW) {
                ally->skipNextMove();
            }
        }
    }
}

bool AllyManager::isValidPosition(int x, int y, int width, int height) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

bool AllyManager::isCellPassable(const Cell& cell) const {
    return cell.isPassable();
}

bool AllyManager::isCellEmpty(int x, int y, const Player& player, const EnemyManager& enemyManager,
                              const BuildingManager& buildingManager, const TowerManager& towerManager,
                              const std::vector<std::vector<Cell>>& grid, int width, int height) const {
    if (player.getPosition().x == x && player.getPosition().y == y) {
        return false;
    }
    
    if (hasAllyOnCell(x, y)) {
        return false;
    }
    
    if (enemyManager.hasEnemyOnCell(x, y)) {
        return false;
    }
    
    if (buildingManager.hasBuildingOnCell(x, y)) {
        return false;
    }
    
    if (towerManager.hasTowerOnCell(x, y)) {
        return false;
    }
    
    return true;
}

