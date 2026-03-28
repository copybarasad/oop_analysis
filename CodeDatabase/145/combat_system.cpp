#include "combat_system.h"
#include "collision_handler.h"
#include <iostream>

CombatSystem::CombatSystem(Player& player, std::vector<Enemy>& enemies, std::vector<EnemyBuilding>& buildings)
    : player(player), enemies(enemies), buildings(buildings) {}

void CombatSystem::performMeleeAttack() {
    bool hitEnemy = false;

    for (auto it = enemies.begin(); it != enemies.end(); ++it) {
        if (!it->isAlive()) continue;  // Пропускаем мёртвых врагов
        
        auto enemyPos = it->getPosition();

        if (player.isEnemyInMeleeRange(enemyPos)) {
            std::cout << "Melee attack! ";
            std::cout << "Player deals " << player.getMeleeDamage() << " damage to enemy at ("
                      << enemyPos.first << "," << enemyPos.second << ")!\n";

            attackEnemy(*it, player.getMeleeDamage(), false);
            hitEnemy = true;

            if (!player.isAlive()) {
                std::cout << "Player died! Game Over!\n";
                return;
            }
        }
    }

    if (hitEnemy) {
        std::cout << "Melee combat completed!\n";
    }
}

void CombatSystem::performRangedAttack() {
    auto playerPos = player.getPosition();
    bool hitEnemy = false;
    bool hitBuilding = false;
    int enemiesHit = 0;
    int buildingsHit = 0;

    std::cout << "Ranged area attack!\n";

    for (auto it = enemies.begin(); it != enemies.end(); ) {
        auto enemyPos = it->getPosition();

        if (player.isEnemyInRangedRange(enemyPos)) {
            std::cout << "Hit enemy at (" << enemyPos.first << "," << enemyPos.second
                      << ") for " << player.getRangedDamage() << " damage! ";
            std::cout << "(Distance: " << (std::abs(enemyPos.first - playerPos.first) +
                          std::abs(enemyPos.second - playerPos.second)) << " cells)\n";

            attackEnemy(*it, player.getRangedDamage(), true);
            hitEnemy = true;
            enemiesHit++;

            if (!it->isAlive()) {
                it = enemies.erase(it);
                continue;
            }
        }
        ++it;
    }

    
    for (auto it = buildings.begin(); it != buildings.end(); ) {
        auto buildingPos = it->getPosition();
        int distance = std::abs(buildingPos.first - playerPos.first) + 
                       std::abs(buildingPos.second - playerPos.second);

        if (distance <= 3 && distance > 0) {
            std::cout << "Hit enemy building at (" << buildingPos.first << "," << buildingPos.second
                      << ") for " << player.getRangedDamage() << " damage! ";
            std::cout << "(Distance: " << distance << " cells)\n";

            it->takeDamage(player.getRangedDamage());
            hitBuilding = true;
            buildingsHit++;

            if (!it->isAlive()) {
                std::cout << "Enemy building destroyed! +20 points\n";
                player.increaseScore(20);
                it = buildings.erase(it);
                continue;
            } else {
                std::cout << "Building HP: " << it->getHealth() << "\n";
            }
        }
        ++it;
    }

    if (hitEnemy || hitBuilding) {
        if (enemiesHit > 0) {
            std::cout << "Hit " << enemiesHit << " enemies";
            if (buildingsHit > 0) std::cout << " and " << buildingsHit << " buildings";
            std::cout << "! No counter damage.\n";
        } else if (buildingsHit > 0) {
            std::cout << "Hit " << buildingsHit << " buildings!\n";
        }
    } else {
        std::cout << "No enemies or buildings in ranged range (up to 3 cells)!\n";
    }
}

void CombatSystem::checkCombat() {
    auto playerPos = player.getPosition();

    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (it->getPosition() == playerPos) {
            if (player.getCombatMode() == CombatMode::MELEE) {
                std::cout << "Melee combat! ";
                std::cout << "Player deals " << player.getMeleeDamage() << " damage to enemy!\n";
                it->takeDamage(player.getMeleeDamage());

                if (!it->isAlive()) {
                    std::cout << "Enemy defeated! +10 points\n";
                    player.increaseScore(10);

                    it = enemies.erase(it);
                    continue;
                } else {
                    std::cout << "Enemy HP: " << it->getHealth() << "/" << it->getMaxHealth() << "\n";
                }

                std::cout << "Enemy counterattacks! ";
                CollisionHandler::handleEnemyPlayerCollision(*it, player);

                if (!player.isAlive()) {
                    std::cout << "Player died! Game Over!\n";
                    return;
                }
            } else {
                std::cout << "Player is in RANGED mode on enemy cell - no automatic combat!\n";
                std::cout << "Use SPACE for ranged attack or switch to MELEE mode with 'M'!\n";
            }
            ++it;
        } else {
            ++it;
        }
    }
}

void CombatSystem::attackEnemy(Enemy& enemy, int damage, bool isRanged) {
    enemy.takeDamage(damage);
    std::cout << "Enemy HP: " << enemy.getHealth() << "/" << enemy.getMaxHealth() << "\n";

    if (!enemy.isAlive()) {
        std::cout << "Enemy defeated! +10 points\n";
        player.increaseScore(10);
        return;
    }

    if (!isRanged) {
        std::cout << "Enemy counterattacks! ";
        CollisionHandler::handleEnemyPlayerCollision(enemy, player);
    }
}