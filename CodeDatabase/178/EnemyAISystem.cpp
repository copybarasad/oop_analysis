#include "EnemyAISystem.h"
#include <iostream>
#include <cmath>

EnemyAISystem::EnemyAISystem(GameField& gameField, Player& gamePlayer, MovementSystem& moveSystem) 
    : field(gameField), player(gamePlayer), movementSystem(moveSystem) {}

void EnemyAISystem::updateEnemies(std::vector<Enemy> &enemies) {
    for (auto &enemy : enemies) {
        if (!enemy.isAlive()) continue;
        
        Position enemyPos = enemy.getPosition();
        Position playerPos = player.getPosition();
        Position direction(0, 0);
        
        if (std::abs(playerPos.x - enemyPos.x) > std::abs(playerPos.y - enemyPos.y)) {
            direction.x = (playerPos.x > enemyPos.x) ? 1 : -1;
        } else {
            direction.y = (playerPos.y > enemyPos.y) ? 1 : -1;
        }
        
        Position newPosition = movementSystem.calculateNewPosition(enemyPos, direction);
        
        if (movementSystem.isValidMove(newPosition)) {
            if (newPosition == player.getPosition()) {
                player.takeDamage(enemy.getDamage());
                std::cout << "Enemy attacked player for " << enemy.getDamage() << " damage!\n";
            } else {
                movementSystem.moveEnemy(enemy, newPosition);
            }
        }
    }
}

void EnemyAISystem::updateTowers(std::vector<EnemyTower> &towers) {
    for (auto &tower : towers) {
        if (!tower.isAlive()) continue;
        if (!tower.canFire()) {
            tower.tickCooldown();
            continue;
        }
        
        Position tpos = tower.getPosition();
        Position ppos = player.getPosition();
        int dist = std::abs(ppos.x - tpos.x) + std::abs(ppos.y - tpos.y);
        
        if (dist <= tower.getRange()) {
            int dmg = tower.getDamage();
            player.takeDamage(dmg);
            std::cout << "Tower at (" << tpos.x << "," << tpos.y << ") hit player for " << dmg << " damage!\n";
            tower.setCooldown(2);
        }
    }
}

Position EnemyAISystem::calculateEnemyMove(const Enemy &enemy) const {
    Position enemyPos = enemy.getPosition();
    Position playerPos = player.getPosition();
    Position direction(0, 0);
    
    if (std::abs(playerPos.x - enemyPos.x) > std::abs(playerPos.y - enemyPos.y)) {
        direction.x = (playerPos.x > enemyPos.x) ? 1 : -1;
    } else {
        direction.y = (playerPos.y > enemyPos.y) ? 1 : -1;
    }
    
    return movementSystem.calculateNewPosition(enemyPos, direction);
}