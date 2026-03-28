#include "CombatSystem.h"
#include <cmath>
#include <iostream>

AttackResult CombatSystem::performPlayerAttack(Player &player, Character &target) {
    if (!target.isAlive()) return AttackResult::NO_ENEMY;
    
    Position targetPos = target.getPosition();
    Position playerPos = player.getPosition();
    int distance = std::abs(playerPos.x - targetPos.x) + std::abs(playerPos.y - targetPos.y);
    int maxRange = player.getAttackRange();
    
    if (distance > maxRange) return AttackResult::OUT_OF_RANGE;
    
    target.takeDamage(player.getDamage());
    player.addScore(10);
    if (!target.isAlive()) {
        player.addScore(20);
        player.addCoins(15);
    }

    return AttackResult::SUCCESS;
}

void CombatSystem::applyDamage(Character &target, int damage) {
    target.takeDamage(damage);
}

bool CombatSystem::isInRange(const Position &attackerPos, const Position &targetPos, int range) {
    int distance = std::abs(attackerPos.x - targetPos.x) + std::abs(attackerPos.y - targetPos.y);
    return distance <= range;
}

void CombatSystem::performAreaAttack(Player &player, std::vector<Enemy> &enemies, std::vector<EnemyTower> &towers) {
    int enemiesHit = 0;
    int attackRange = player.getAttackRange();
    
    for (auto &enemy : enemies) {
        if (!enemy.isAlive()) continue;
        if (isInRange(player.getPosition(), enemy.getPosition(), attackRange)) {
            enemy.takeDamage(player.getDamage());
            enemiesHit++;
            player.addScore(10);
            if (!enemy.isAlive()) {
                player.addScore(20);
                player.addCoins(15);
            }
        }
    }
    
    std::cout << "Area attack hit " << enemiesHit << " enemies!\n";
}

void CombatSystem::performRangedAttack(Player &player, const Position &direction, std::vector<Enemy> &enemies, GameField &field) {
    Position playerPos = player.getPosition();
    int range = player.getAttackRange();
    bool hitEnemy = false;
    
    for (int i = 1; i <= range; i++) {
        Position checkPos(playerPos.x + direction.x * i, playerPos.y + direction.y * i);
        if (!field.isValidPosition(checkPos)) continue;
        
        for (auto &enemy : enemies) {
            if (enemy.isAlive() && enemy.getPosition() == checkPos) {
                enemy.takeDamage(player.getDamage());
                player.addScore(10);
                hitEnemy = true;
                if (!enemy.isAlive()) {
                    player.addScore(20);
                    player.addCoins(15);
                }
                break;
            }
        }
        
        if (hitEnemy) break;
    }
    
    if (!hitEnemy) {
        std::cout << "No enemies in attack direction within range " << range << "!\n";
    }
}