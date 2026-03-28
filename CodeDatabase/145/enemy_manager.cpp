#include "enemy_manager.h"
#include "enemy_building.h"
#include "enemy_tower.h"
#include "ally.h"
#include <iostream>

EnemyManager::EnemyManager(std::vector<Enemy>& enemies, GameField& field, Player& player,
                           std::vector<Ally>& allies)
    : enemies(enemies), field(field), player(player), allies(allies) {}

void EnemyManager::moveEnemies(const std::vector<EnemyBuilding>& buildings,
                               const std::vector<EnemyTower>& towers) {
    for (auto& enemy : enemies) {
        if (!enemy.isAlive()) continue;

        auto oldPos = enemy.getPosition();
        auto newPos = enemy.calculateMoveTowardsPlayer(player.getPosition());

        if (newPos == player.getPosition()) {
            continue;
        }

        if (field.isSlowing(newPos.first, newPos.second)) {
            continue;
        }

        if (field.isStructureAt(newPos.first, newPos.second, buildings, towers)) {
            continue;
        }

        bool allyBlocked = false;
        for (auto& ally : allies) {
            if (ally.isAlive() && newPos == ally.getPosition()) {
                allyBlocked = true;
                break;
            }
        }
        if (allyBlocked) {
            continue;
        }

        bool enemyOnNewPos = false;
        for (auto& other : enemies) {
            if (&enemy == &other) continue;
            if (other.isAlive() && other.getPosition() == newPos) {
                enemyOnNewPos = true;
                break;
            }
        }
        if (enemyOnNewPos) {
            continue;
        }

        if (field.isValidPosition(newPos.first, newPos.second) &&
            field.isPassable(newPos.first, newPos.second)) {
            enemy.setPosition(newPos.first, newPos.second);
            checkTrapsAtPosition(enemy);
        }
    }
}

bool EnemyManager::hasEnemiesRemaining() const {
    return !enemies.empty();
}

bool EnemyManager::isEnemyStillAlive(const Enemy& enemy) const {
    return enemy.isAlive();
}

void EnemyManager::checkTrapsAtPosition(Enemy& enemy) {
    auto pos = enemy.getPosition();

    if (field.hasTrapAt(pos.first, pos.second)) {
        int damage = field.getTrapDamageAt(pos.first, pos.second);
        std::cout << "Enemy stepped on trap at (" << pos.first << "," << pos.second
                  << ") and takes " << damage << " damage!\n";
        enemy.takeDamage(damage);
        field.removeTrap(pos.first, pos.second);

        if (enemy.isAlive()) {
            std::cout << "Enemy HP: " << enemy.getHealth() << "/" << enemy.getMaxHealth() << "\n";
        } else {
            std::cout << "Enemy defeated by trap! +10 points\n";
            player.increaseScore(10);
        }
    }
}

void EnemyManager::processBuildingSpawns(std::vector<EnemyBuilding>& buildings,
                                        const std::vector<EnemyTower>& towers) {
    for (auto& building : buildings) {
        if (building.shouldSpawnEnemy()) {
            Enemy newEnemy = building.spawnEnemy();
            auto pos = newEnemy.getPosition();
            if (!field.isOccupied(pos.first, pos.second, enemies, buildings, towers, nullptr, nullptr)
                && field.isValidPosition(pos.first, pos.second) && field.isPassable(pos.first, pos.second)) {
                enemies.push_back(newEnemy);
                std::cout << "Enemy building spawned a new enemy!\n";
                building.setSpawnCounter(0);
            }
        }
    }
}