#include "enemy_manager.h"
#include "player.h"
#include "player.h"
#include "combat_manager.h"
#include <random>
#include <algorithm>

EnemyManager::EnemyManager(GameField& field, int health, int damage) 
    : field(field), enemyHealth(health), enemyDamage(damage) {}

void EnemyManager::setEnemyParams(int health, int damage) {
    enemyHealth = health;
    enemyDamage = damage;
}

void EnemyManager::spawnEnemyAtPosition(const Position& pos) {
    Enemy newEnemy(enemyHealth, enemyDamage);
    newEnemy.setPosition(pos);
    addEnemy(newEnemy);
}

const std::vector<Enemy>& EnemyManager::getEnemies() const {
    return enemies;
}

void EnemyManager::addEnemy(const Enemy& enemy) {
    enemies.push_back(enemy);
    field.setCellOccupied(enemy.getPosition(), true);
}

void EnemyManager::moveEnemy(Enemy& enemy, Player& player, CombatManager& combatManager) {
    Position currentPos = enemy.getPosition();
    auto adjacentPositions = field.getAdjacentPositions(currentPos);
    
    if (!adjacentPositions.empty()) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, adjacentPositions.size() - 1);
        
        Position newPos = adjacentPositions[dist(gen)];
        
        // ПРОВЕРЯЕМ ЛОВУШКИ ПЕРЕД ДВИЖЕНИЕМ
        combatManager.checkTrapTrigger(newPos, enemies, player);
        
        if (newPos == player.getPosition()) {
            player.takeDamage(enemy.getDamage()); 
        } else if (!field.getCell(newPos).getIsOccupied()) {
            field.setCellOccupied(currentPos, false);
            enemy.setPosition(newPos);
            field.setCellOccupied(newPos, true);
        }
    }
}

void EnemyManager::processEnemyTurns(Player& player, CombatManager& combatManager) {
    for (auto& enemy : enemies) {
        if (enemy.isAlive()) {
            moveEnemy(enemy, player, combatManager);
        }
    }
}

void EnemyManager::removeDeadEnemies(Player& player) {
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(), 
                      [this, &player](Enemy& enemy) {
                          if (!enemy.isAlive()) {
                              field.setCellOccupied(enemy.getPosition(), false);
                              return true;
                          }
                          return false;
                      }),
        enemies.end()
    );
}

void EnemyManager::clearEnemies() {
    for (auto& enemy : enemies) {
        field.setCellOccupied(enemy.getPosition(), false);
    }
    enemies.clear();
}

void EnemyManager::processTrapDamage(const Position& position, int damage) {
    for (auto& enemy : enemies) {
        if (enemy.getPosition() == position && enemy.isAlive()) {
            enemy.takeDamage(damage);
            break;
        }
    }
}

void EnemyManager::spawnEnemy(const Enemy& enemy) {
    enemies.push_back(enemy);
    field.setCellOccupied(enemy.getPosition(), true);
}