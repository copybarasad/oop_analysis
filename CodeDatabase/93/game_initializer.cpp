#include "game_initializer.h"
#include <random>
#include <algorithm>

void GameInitializer::initializeEnemies(std::vector<Enemy>& enemies, const GameField& field, int enemyCount) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(1, field.getWidth() - 1);
    std::uniform_int_distribution<> disY(1, field.getHeight() - 1);
    
    int enemiesCreated = 0;
    const int maxAttempts = 200;
    
    while (enemiesCreated < enemyCount) {
        int x = disX(gen);
        int y = disY(gen);
        
        if (field.isPositionPassable(x, y) && !(x == 0 && y == 0)) {
            bool positionOccupied = false;
            for (const auto& enemy : enemies) {
                Position enemyPos = enemy.getPosition();
                if (enemyPos.x == x && enemyPos.y == y) {
                    positionOccupied = true;
                    break;
                }
            }
            
            if (!positionOccupied) {
                enemies.emplace_back(x, y);
                enemiesCreated++;
            }
        }
        
        static int attempts = 0;
        if (++attempts > maxAttempts) {
            for (int y = 0; y < field.getHeight() && enemiesCreated < enemyCount; ++y) {
                for (int x = 0; x < field.getWidth() && enemiesCreated < enemyCount; ++x) {
                    if (field.isPositionPassable(x, y) && !(x == 0 && y == 0)) {
                        bool positionOccupied = false;
                        for (const auto& enemy : enemies) {
                            Position enemyPos = enemy.getPosition();
                            if (enemyPos.x == x && enemyPos.y == y) {
                                positionOccupied = true;
                                break;
                            }
                        }
                        if (!positionOccupied) {
                            enemies.emplace_back(x, y);
                            enemiesCreated++;
                        }
                    }
                }
            }
            break;
        }
    }
}

void GameInitializer::initializeTowers(std::vector<EnemyTower>& towers, const GameField& field, int count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(1, field.getWidth() - 2);
    std::uniform_int_distribution<> disY(1, field.getHeight() - 2);

    int placed = 0;
    for (int attempts = 0; attempts < 100 && placed < count; ++attempts) {
        int x = disX(gen), y = disY(gen);
        if (field.isPositionPassable(x, y) && !(x == 0 && y == 0)) {
            bool conflict = false;
            for (const auto& tower : towers) {
                if (tower.getPosition().x == x && tower.getPosition().y == y) {
                    conflict = true; break;
                }
            }
            if (!conflict) {
                towers.emplace_back(x, y);
                placed++;
            }
        }
    }
}

void GameInitializer::validateEnemies(std::vector<Enemy>& enemies, const GameField& field) {
    auto it = std::remove_if(enemies.begin(), enemies.end(), [&](const Enemy& enemy) {
        return !field.isPositionPassable(enemy.getPosition().x, enemy.getPosition().y) || 
               (enemy.getPosition().x == 0 && enemy.getPosition().y == 0) || 
               !enemy.isAlive();
    });
    enemies.erase(it, enemies.end());
}

void GameInitializer::validateTowers(std::vector<EnemyTower>& towers, const GameField& field) {
    auto it = std::remove_if(towers.begin(), towers.end(), [&](const EnemyTower& tower) {
        return !field.isPositionPassable(tower.getPosition().x, tower.getPosition().y) || 
               (tower.getPosition().x == 0 && tower.getPosition().y == 0);
    });
    towers.erase(it, towers.end());
}