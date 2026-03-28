#include "../include/CombatSystem.h"
#include "../include/BuildingManager.h"
#include "../include/TowerManager.h"
#include "../include/AllyManager.h"
#include "../include/Ally.h"
#include <cmath>

CombatSystem::CombatSystem(const PathfindingService& pathfinding)
    : pathfinding(pathfinding) {}

bool CombatSystem::movePlayer(Direction direction, Player& player, Position& playerPosition, 
                              const MapGrid& grid, const EnemyManager& enemyManager) {
    Position newPos = playerPosition;
    
    switch (direction) {
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
            return false;
    }
    
    if (!grid.canMoveTo(newPos) || enemyManager.isPositionOccupiedByEnemy(newPos)) {
        return false;
    }
    
    playerPosition = newPos;
    
    if (grid.getCellType(newPos) == CellType::SLOWING) {
        player.setSlow(true);
    }
    
    return true;
}

void CombatSystem::moveEnemies(Player& player, const Position& playerPosition, 
                               const MapGrid& grid, EnemyManager& enemyManager,
                               const BuildingManager& buildingManager,
                               const TowerManager& towerManager,
                               AllyManager& allyManager) {
    auto& enemies = enemyManager.getEnemies();
    
    for (auto& enemyPair : enemies) {
        if (!enemyPair.second->isAlive()) {
            continue;
        }
        
        Position enemyPos = enemyPair.first;
        int dx = playerPosition.getX() - enemyPos.getX();
        int dy = playerPosition.getY() - enemyPos.getY();
        
        if ((abs(dx) == 1 && dy == 0) || (abs(dy) == 1 && dx == 0)) {
            player.takeDamage(enemyPair.second->getDamage());
            continue;
        }
        
        bool attackedAlly = false;
        for (auto& allyPair : allyManager.getAllies()) {
            if (!allyPair.second->isAlive()) {
                continue;
            }
            
            int allyDx = allyPair.first.getX() - enemyPos.getX();
            int allyDy = allyPair.first.getY() - enemyPos.getY();
            
            if ((abs(allyDx) == 1 && allyDy == 0) || (abs(allyDy) == 1 && allyDx == 0)) {
                int enemyDamage = enemyPair.second->getDamage();
                allyPair.second->takeDamage(enemyDamage);
                attackedAlly = true;
                break;
            }
        }
        
        if (attackedAlly) {
            continue;
        }
        
        Direction moveDirection = pathfinding.findBestMoveTowardsTarget(enemyPos, playerPosition, grid, enemyManager);
        
        if (moveDirection == Direction::NONE) {
            continue;
        }
        
        Position newPos = enemyPos;
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
            newPos != playerPosition && !buildingManager.hasBuildingAt(newPos) && 
            !towerManager.hasTowerAt(newPos) && !allyManager.isPositionOccupiedByAlly(newPos)) {
            enemyPair.first = newPos;
        }
    }
}

int CombatSystem::attackInDirection(Direction direction, int damage, bool isRanged,
                                   const Position& playerPosition, const MapGrid& grid,
                                   EnemyManager& enemyManager, int& outTotalDamage) {
    int hitCount = 0;
    outTotalDamage = 0;
    Position currentPos = playerPosition;
    
    int dx = 0, dy = 0;
    switch (direction) {
        case Direction::UP:
            dy = -1;
            break;
        case Direction::DOWN:
            dy = 1;
            break;
        case Direction::LEFT:
            dx = -1;
            break;
        case Direction::RIGHT:
            dx = 1;
            break;
        case Direction::NONE:
            return 0;
    }
    
    if (!isRanged) {
        Position attackPos(currentPos.getX() + dx, currentPos.getY() + dy);
        if (enemyManager.hasAliveEnemyAt(attackPos)) {
            outTotalDamage += enemyManager.attackEnemyAt(attackPos, damage);
            hitCount++;
        }
        return hitCount;
    }
    
    while (true) {
        currentPos.setX(currentPos.getX() + dx);
        currentPos.setY(currentPos.getY() + dy);
        
        if (!grid.isValidPosition(currentPos)) {
            break;
        }
        
        if (grid.getCellType(currentPos) == CellType::WALL) {
            break;
        }
        
        if (enemyManager.hasAliveEnemyAt(currentPos)) {
            outTotalDamage += enemyManager.attackEnemyAt(currentPos, damage);
            hitCount++;
        }
    }
    
    return hitCount;
}
