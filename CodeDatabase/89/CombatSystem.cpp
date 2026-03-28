#include "CombatSystem.h"
#include "MovementSystem.h"
#include <iostream>
#include <cmath>

CombatSystem::CombatSystem(GameGrid& grid) : grid(grid) {}

void CombatSystem::playerAttack(Player& player, Direction direction, std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (!player.isAlive()) {
        std::cout << "Player is not alive or not present to attack." << std::endl;
        return;
    }

    int targetX, targetY;
    Enemy* targetEnemy = findEnemyInDirection(player, direction, targetX, targetY, enemies);

    if (targetEnemy) {
        int damage = player.getDamage();
        std::cout << "Player attacks enemy at (" << targetX << ", " << targetY << ") with " << damage << " damage." << std::endl;
        targetEnemy->takeDamage(damage);
        std::cout << "Enemy health: " << targetEnemy->getHealth() << std::endl;

        if (!targetEnemy->isAlive()) {
            std::cout << "Enemy at (" << targetX << ", " << targetY << ") defeated!" << std::endl;
            player.addScore(100);
            grid.updateCell(targetX, targetY, CellType::Empty);
        }
    } else {
        std::cout << "No enemy to attack in that direction." << std::endl;
    }
}

bool CombatSystem::hasLineOfSight(int fromX, int fromY, int toX, int toY) const {
    int dx = toX - fromX;
    int dy = toY - fromY;
    
    int steps = std::max(std::abs(dx), std::abs(dy));
    
    if (steps <= 1) return true;
    
    for (int i = 1; i < steps; ++i) {
        int checkX = fromX + (dx * i) / steps;
        int checkY = fromY + (dy * i) / steps;
        
        if (checkX == toX && checkY == toY) break;
        
        if (!grid.isCellPassable(checkX, checkY) && !(checkX == toX && checkY == toY)) {
            return false;
        }
    }
    return true;
}

Enemy* CombatSystem::findEnemyInDirection(Player& player, Direction direction, int& targetX, int& targetY, std::vector<std::unique_ptr<Enemy>>& enemies) {
    int playerX = player.getPosition().first;
    int playerY = player.getPosition().second;
    
    if (player.getCombatMode() == CombatMode::Melee) {
        targetX = playerX;
        targetY = playerY;
        
        switch (direction) {
            case Direction::Up:    targetY--; break;
            case Direction::Down:  targetY++; break;
            case Direction::Left:  targetX--; break;
            case Direction::Right: targetX++; break;
        }
        
        for (auto& enemy : enemies) {
            if (enemy && enemy->getPosition().first == targetX && enemy->getPosition().second == targetY) {
                return enemy.get();
            }
        }
    } 
    else {
        for (int distance = 1; distance <= 2; ++distance) {
            targetX = playerX;
            targetY = playerY;
            
            switch (direction) {
                case Direction::Up:    targetY -= distance; break;
                case Direction::Down:  targetY += distance; break;
                case Direction::Left:  targetX -= distance; break;
                case Direction::Right: targetX += distance; break;
            }
            
            if (!grid.isValidCoordinate(targetX, targetY)) {
                continue;
            }
            
            if (!hasLineOfSight(playerX, playerY, targetX, targetY)) {
                continue;
            }
            
            for (auto& enemy : enemies) {
                if (enemy && enemy->getPosition().first == targetX && enemy->getPosition().second == targetY) {
                    return enemy.get();
                }
            }
        }
    }
    
    return nullptr;
}