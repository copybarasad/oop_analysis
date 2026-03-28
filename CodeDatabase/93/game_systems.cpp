#include "game_systems.h"
#include <random>
#include <algorithm>
#include <iostream>

void EnemyAI::processEnemyTurns(std::vector<Enemy>& enemies, Player& player, const GameField& field, GameController& game) {
    for (auto& enemy : enemies) {
        if (enemy.isAlive()) {
            moveEnemyTowardsPlayer(enemy, player, field, enemies, game);
        }
    }
}

void EnemyAI::moveEnemyTowardsPlayer(Enemy& enemy, Player& player, const GameField& field, const std::vector<Enemy>& allEnemies, GameController& game) {
    Position enemyPos = enemy.getPosition();
    Position playerPos = player.getPosition();
    
    int diffX = playerPos.x - enemyPos.x;
    int diffY = playerPos.y - enemyPos.y;
    
    int newX = enemyPos.x;
    int newY = enemyPos.y;
    
    if (std::abs(diffX) > std::abs(diffY)) {
        if (diffX > 0) {
            newX = enemyPos.x + 1; 
        } else if (diffX < 0) {
            newX = enemyPos.x - 1; 
        }
    } else {
        if (diffY > 0) {
            newY = enemyPos.y + 1; 
        } else if (diffY < 0) {
            newY = enemyPos.y - 1; 
        }
    }
    
    if (field.isPositionPassable(newX, newY)) {
        if (newX == playerPos.x && newY == playerPos.y) {
            player.takeDamage(enemy.getDamage());
        } else {
            bool positionOccupied = std::any_of(allEnemies.begin(), allEnemies.end(),
                [newX, newY, &enemy](const Enemy& other) {
                    if (&other == &enemy) return false;
                    Position otherPos = other.getPosition();
                    return otherPos.x == newX && otherPos.y == newY && other.isAlive();
                });

            bool towerHere = std::any_of(game.getTowers().begin(), game.getTowers().end(),
                [newX, newY](const EnemyTower& t) {
                    return t.getPosition().x == newX && t.getPosition().y == newY;
                });

            if (!positionOccupied && !towerHere) {
                enemy.move(newX - enemyPos.x, newY - enemyPos.y);
            }
        }
    } else {
        int altX = enemyPos.x;
        int altY = enemyPos.y;
        
        if (std::abs(diffX) > std::abs(diffY)) {
            if (diffY > 0) {
                altY = enemyPos.y + 1;
            } else if (diffY < 0) {
                altY = enemyPos.y - 1;
            }
        } else {
            if (diffX > 0) {
                altX = enemyPos.x + 1;
            } else if (diffX < 0) {
                altX = enemyPos.x - 1;
            }
        }
        
        if (field.isPositionPassable(altX, altY) && 
            !(altX == playerPos.x && altY == playerPos.y)) {
            
            bool positionOccupied = std::any_of(allEnemies.begin(), allEnemies.end(),
                [altX, altY, &enemy](const Enemy& other) {
                    if (&other == &enemy) return false;
                    Position otherPos = other.getPosition();
                    return otherPos.x == altX && otherPos.y == altY && other.isAlive();
                });
            
            bool towerHere = std::any_of(game.getTowers().begin(), game.getTowers().end(),
                [altX, altY](const EnemyTower& t) {
                    return t.getPosition().x == altX && t.getPosition().y == altY;
                });

            if (!positionOccupied && !towerHere) {
                enemy.move(altX - enemyPos.x, altY - enemyPos.y);
            }
        }
    }

    Position newPos = enemy.getPosition();
    auto& mutableField = const_cast<GameField&>(field);
    for (auto& trap : mutableField.getTraps()) {
        if (trap.isActive() && trap.getPosition() == newPos) {
            enemy.takeDamage(trap.getDamage());
            trap.deactivate();
        }
    }
}

bool CombatSystem::performMeleeAttack(Player& player, std::vector<Enemy>& enemies, int targetX, int targetY) {
    auto enemyIt = std::find_if(enemies.begin(), enemies.end(),
        [targetX, targetY](const Enemy& enemy) {
            Position enemyPos = enemy.getPosition();
            return enemyPos.x == targetX && enemyPos.y == targetY && enemy.isAlive();
        });
    
    if (enemyIt != enemies.end()) {
        enemyIt->takeDamage(player.getDamage());
        if (!enemyIt->isAlive()) player.addScore(30);
        return true;
    }
    return false;
}

bool CombatSystem::performRangedAttack(Player& player, std::vector<Enemy>& enemies, const GameField& field, int directionX, int directionY) {
    Position playerPos = player.getPosition();
    int attackRange = player.getAttackRange();
    
    for (int distance = 1; distance <= attackRange; ++distance) {
        int targetX = playerPos.x + directionX * distance;
        int targetY = playerPos.y + directionY * distance;
        
        if (!field.isPositionValid(targetX, targetY)) break;
        if (!field.isPositionPassable(targetX, targetY)) break;
        
        auto enemyIt = std::find_if(enemies.begin(), enemies.end(),
            [targetX, targetY](const Enemy& enemy) {
                Position enemyPos = enemy.getPosition();
                return enemyPos.x == targetX && enemyPos.y == targetY && enemy.isAlive();
            });
        
        if (enemyIt != enemies.end()) {
            enemyIt->takeDamage(player.getDamage());
            if (!enemyIt->isAlive()) player.addScore(30);
            return true;
        }
    }
    return false;
}

bool GameStateChecker::isGameWon(const std::vector<Enemy>& enemies, bool gameRunning, const Player& player) {
    return !gameRunning && player.isAlive();
}

bool GameStateChecker::areAllEnemiesDefeated(const std::vector<Enemy>& enemies) {
    return std::all_of(enemies.begin(), enemies.end(), 
        [](const Enemy& enemy) { return !enemy.isAlive(); });
}