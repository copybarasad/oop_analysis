#include "CombatSystem.h"
#include "Player.h"
#include "Enemy.h"
#include "GameField.h"
#include "MovementValidator.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <climits>

bool CombatSystem::performMeleeAttack(Player& player, const Position& target, std::vector<Enemy>& enemies) {
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (enemies[i].isAlive() && enemies[i].getPosition() == target) {
            int damage = player.getDamage();
            enemies[i].takeDamage(damage);
            
            if (!enemies[i].isAlive()) {
                player.addScore(10);
                std::cout << "Ближняя атака! Враг побежден! +10 очков" << std::endl;
            } else {
                std::cout << "Ближняя атака! Нанесено урона врагу: " << damage << std::endl;
            }
            return true;
        }
    }
    
    return false;
}

bool CombatSystem::performRangedAttack(Player& player, const std::vector<Enemy>& enemies,
                                      const GameField& field) {
    Position playerPos = player.getPosition();
    Position nearestEnemyPos = findNearestEnemy(playerPos, enemies, field);
    
    if (nearestEnemyPos.x == -1 && nearestEnemyPos.y == -1) {
        return false;
    }
    
    if (!canAttack(playerPos, nearestEnemyPos, CombatMode::RANGED, field)) {
        std::cout << "Дальняя атака невозможна - нет подходящей цели в зоне поражения" << std::endl;
        return false;
    }
    
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (enemies[i].isAlive() && enemies[i].getPosition() == nearestEnemyPos) {
            int damage = player.getDamage();
            const_cast<Enemy&>(enemies[i]).takeDamage(damage);
            
            if (!enemies[i].isAlive()) {
                player.addScore(10);
                std::cout << "Дальняя атака! Враг побежден! +10 очков" << std::endl;
            } else {
                std::cout << "Дальняя атака! Нанесено урона врагу: " << damage << std::endl;
            }
            return true;
        }
    }
    
    return false;
}

Position CombatSystem::findNearestEnemy(const Position& playerPos, const std::vector<Enemy>& enemies,
                                       const GameField& field) {
    int minDistance = INT_MAX;
    Position nearestEnemy(-1, -1);
    
    for (const auto& enemy : enemies) {
        if (!enemy.isAlive()) continue;
        
        Position enemyPos = enemy.getPosition();
        int distance = std::abs(playerPos.x - enemyPos.x) + std::abs(playerPos.y - enemyPos.y);
        
        if (distance <= CombatModeUtils::getAttackRange(CombatMode::RANGED) &&
            distance > 0 && 
            hasLineOfSight(playerPos, enemyPos, field) &&
            distance < minDistance) {
            
            minDistance = distance;
            nearestEnemy = enemyPos;
        }
    }
    
    return nearestEnemy;
}

void CombatSystem::performEnemyAttack(Enemy& enemy, Player& player) {
    player.takeDamage(enemy.getDamage());
}

bool CombatSystem::canAttack(const Position& attackerPos, const Position& targetPos, 
                            CombatMode mode, const GameField& field) {
    int distance = std::abs(attackerPos.x - targetPos.x) + std::abs(attackerPos.y - targetPos.y);
    int maxRange = CombatModeUtils::getAttackRange(mode);
    
    if (distance > maxRange || distance == 0) {
        return false;
    }
    
    if (CombatModeUtils::requiresLineOfSight(mode)) {
        if (attackerPos.x != targetPos.x && attackerPos.y != targetPos.y) {
            return false;
        }
        return hasLineOfSight(attackerPos, targetPos, field);
    }
    
    return true;
}

bool CombatSystem::hasLineOfSight(const Position& from, const Position& to, const GameField& field) {
    if (from.x != to.x && from.y != to.y) {
        return false;
    }
    
    if (from.x == to.x) {
        int startY = std::min(from.y, to.y);
        int endY = std::max(from.y, to.y);
        
        for (int y = startY + 1; y < endY; ++y) {
            Position checkPos(from.x, y);
            if (!field.isCellPassable(checkPos)) {
                return false;
            }
        }
    } else {
        int startX = std::min(from.x, to.x);
        int endX = std::max(from.x, to.x);
        
        for (int x = startX + 1; x < endX; ++x) {
            Position checkPos(x, from.y);
            if (!field.isCellPassable(checkPos)) {
                return false;
            }
        }
    }
    
    return true;
}