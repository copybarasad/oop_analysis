#include "CombatSystem.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include <algorithm>
#include <iostream>

CombatSystem::CombatSystem(std::shared_ptr<TextSystem> textSystem) 
    : textSystem(textSystem) {}

bool CombatSystem::playerAttack(Player& player, GameField& field,
                              std::vector<Enemy>& enemies,
                              std::vector<EnemyBuilding>& buildings,
                              std::vector<EnemyTower>& towers) {
    (void)field;

    double attackRange = player.getAttackRange();
    int currentDamage = player.getCurrentDamage();
    bool attacked = false;

    // Атака врагов
    for (auto& enemy : enemies) {
        if (enemy.isAlive() && player.distanceTo(enemy) <= attackRange) {
            enemy.takeDamage(currentDamage);
            if (!enemy.isAlive()) {
                player.setScore(player.getScore() + 10);
                std::cout << textSystem->getEnemyKilledMessage() << std::endl;
                // Убираем врага с поля
                field.removeObjectFromPosition(enemy.getPosition());
            }
            attacked = true;
            break;
        }
    }
    
    if (attacked) return true;

    // Атака зданий
    for (auto& building : buildings) {
        if (building.isAlive() && player.distanceTo(building) <= attackRange) {
            building.takeDamage(currentDamage);
            if (!building.isAlive()) {
                player.setScore(player.getScore() + 25);
                std::cout << textSystem->getBuildingDestroyedMessage() << std::endl;
                // Убираем с поля
                field.removeObjectFromPosition(building.getPosition());
            }
            attacked = true;
            break;
        }
    }
    
    if (attacked) return true;

    // Атака башен
    for (auto& tower : towers) {
        if (tower.isAlive() && player.distanceTo(tower) <= attackRange) {
            tower.takeDamage(currentDamage);
            if (!tower.isAlive()) {
                player.setScore(player.getScore() + 35);
                std::cout << textSystem->getTowerDestroyedMessage() << std::endl;
                field.removeObjectFromPosition(tower.getPosition());
            }
            attacked = true;
            break;
        }
    }
    
    return attacked;
}

void CombatSystem::enemyAttack(Enemy& enemy, Player& player) {
    if (!player.isAlive()) return;

    double attackRange = 1.5;
    if (enemy.distanceTo(player) <= attackRange) {
        int enemyDamage = enemy.getDamage();
        player.takeDamage(enemyDamage);
    }
}