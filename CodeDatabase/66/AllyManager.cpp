#include "../include/AllyManager.h"
#include "../include/MapGrid.h"
#include "../include/EnemyManager.h"
#include "../include/PathfindingService.h"
#include "../include/BuildingManager.h"
#include "../include/TowerManager.h"
#include "../include/Direction.h"
#include <cmath>

void AllyManager::addAlly(const Position& pos, std::unique_ptr<Ally> ally) {
    allies.push_back({pos, std::move(ally)});
}

bool AllyManager::isPositionOccupiedByAlly(const Position& pos) const {
    for (const auto& allyPair : allies) {
        if (allyPair.first == pos && allyPair.second->isAlive()) {
            return true;
        }
    }
    return false;
}

int AllyManager::removeDeadAllies() {
    int removedCount = 0;
    for (auto it = allies.begin(); it != allies.end(); ) {
        if (!it->second->isAlive()) {
            it = allies.erase(it);
            removedCount++;
        } else {
            ++it;
        }
    }
    return removedCount;
}

void AllyManager::moveAllies(const Position& playerPosition, const MapGrid& grid, 
                             const EnemyManager& enemyManager, EnemyManager& enemyManagerMutable,
                             const PathfindingService& pathfinding,
                             const BuildingManager& buildingManager,
                             const TowerManager& towerManager) {
    for (auto& allyPair : allies) {
        if (!allyPair.second->isAlive()) {
            continue;
        }
        
        Position allyPos = allyPair.first;
        
        Position closestEnemyPos;
        int closestDistance = 999999;
        bool foundEnemy = false;
        
        for (const auto& enemyPair : enemyManager.getEnemies()) {
            if (!enemyPair.second->isAlive()) {
                continue;
            }
            
            int dx = enemyPair.first.getX() - allyPos.getX();
            int dy = enemyPair.first.getY() - allyPos.getY();
            int distance = abs(dx) + abs(dy);
            
            if (distance < closestDistance) {
                closestDistance = distance;
                closestEnemyPos = enemyPair.first;
                foundEnemy = true;
            }
        }
        
        if (!foundEnemy) {
            continue;
        }
        
        int dx = closestEnemyPos.getX() - allyPos.getX();
        int dy = closestEnemyPos.getY() - allyPos.getY();
        
        if ((abs(dx) == 1 && dy == 0) || (abs(dy) == 1 && dx == 0)) {
            int damage = allyPair.second->getDamage();
            enemyManagerMutable.attackEnemyAt(closestEnemyPos, damage);
            continue;
        }
        
        Direction moveDirection = pathfinding.findBestMoveTowardsTarget(allyPos, closestEnemyPos, grid, enemyManager);
        
        if (moveDirection == Direction::NONE) {
            continue;
        }
        
        Position newPos = allyPos;
        switch (moveDirection) {
            case Direction::UP:
                newPos.setY(newPos.getY() - 1);
                break;
            case Direction::DOWN:
                newPos.setY(newPos.getY() + 1);
                break;
            case Direction::LEFT:
                newPos.setX(newPos.getX() - 1);
                break;
            case Direction::RIGHT:
                newPos.setX(newPos.getX() + 1);
                break;
            case Direction::NONE:
                continue;
        }
        
        if (grid.canMoveTo(newPos) && !enemyManager.isPositionOccupiedByEnemy(newPos) && 
            !isPositionOccupiedByAlly(newPos) && newPos != playerPosition &&
            !buildingManager.hasBuildingAt(newPos) && !towerManager.hasTowerAt(newPos)) {
            allyPair.first = newPos;
        }
    }
}

const std::vector<std::pair<Position, std::unique_ptr<Ally>>>& AllyManager::getAllies() const {
    return allies;
}

std::vector<std::pair<Position, std::unique_ptr<Ally>>>& AllyManager::getAllies() {
    return allies;
}
