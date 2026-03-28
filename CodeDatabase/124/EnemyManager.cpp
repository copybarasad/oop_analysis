#include "EnemyManager.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

EnemyManager::EnemyManager() : enemiesDefeated(0), totalEnemiesSpawned(0), currentLevel(1) {
    std::srand(std::time(0));
}

void EnemyManager::updateEnemy(Enemy& enemy, GameField& field, Player& player) {
    if (enemy.isAlive()) {
        enemy.moveTowardsPlayer(field, player);
    } else { 
        int enemyX, enemyY;
        if (field.findEnemyPosition(enemyX, enemyY)) {
            field.clearCell(enemyX, enemyY);
        }
        
        enemiesDefeated++;
        std::cout << "Враг побежден! " << enemiesDefeated << "/" << max_enemies_to_win << " для победы\n";
         
        player.getEconomy().addScore(50);
        player.getEconomy().addMana(25);
        player.getEconomy().addEnemyDefeated();
        
        if (enemiesDefeated < max_enemies_to_win) {
            spawnNewEnemy(enemy, field, player);
        }  
    }
}

void EnemyManager::spawnNewEnemy(Enemy& enemy, GameField& field, const Player& player) {
    if (!canSpawnEnemy()) return;
    
    int playerX, playerY;
    field.findPlayerPosition(playerX, playerY);
    
    std::vector<std::pair<int, int>> possiblePositions = findSpawnPositions(field, playerX, playerY);
    
    if (!possiblePositions.empty()) {
        int randomIndex = rand() % possiblePositions.size();
        int spawnX = possiblePositions[randomIndex].first;
        int spawnY = possiblePositions[randomIndex].second;
        
        enemy = Enemy(currentLevel);  
        if (field.placeEnemy(spawnX, spawnY)) {
            totalEnemiesSpawned++;
            std::cout << "Появился новый враг (Уровень " << currentLevel << ")! ";
            std::cout << "Всего побеждено: " << enemiesDefeated << "\n";
            std::cout << "Здоровье: " << enemy.getHealth() << ", Урон: " << enemy.getDamage() << "\n";
            
            
        }
    }
}

bool EnemyManager::canSpawnEnemy() const {
    return totalEnemiesSpawned < max_enemies_to_win;
}

bool EnemyManager::isGameWon() const {
    return enemiesDefeated >= max_enemies_to_win && currentLevel >= total_levels;
}

bool EnemyManager::isLevelComplete() const {
    return enemiesDefeated >= max_enemies_to_win;
}

int EnemyManager::getEnemiesDefeated() const {
    return enemiesDefeated;
}

void EnemyManager::setLevel(int level) {
    currentLevel = level;
    
}

int EnemyManager::getCurrentLevel() const {
    return currentLevel;
}

int EnemyManager::getTotalLevels() const {
    return total_levels;
}

void EnemyManager::resetForNewLevel() {
    enemiesDefeated = 0;
    totalEnemiesSpawned = 0;
}
 
void EnemyManager::placeTowerOnEmptyCell(GameField& field) {
    std::vector<std::pair<int, int>> possiblePositions = {
        {3, 3}, {10, 12}, {7, 7}, {2, 10}, {12, 3}, {8, 8}, {4, 12}
    };
    
    for (int i = 0; i < possiblePositions.size(); i++) {
        int randomIndex = rand() % possiblePositions.size();
        std::swap(possiblePositions[i], possiblePositions[randomIndex]);
    }
    
    for (const auto& pos : possiblePositions) {
        if (field.placeTower(pos.first, pos.second)) {
            return;
        }
    }
    
    for (int y = 0; y < field.getHeight(); y++) {
        for (int x = 0; x < field.getWidth(); x++) {
            if (field.placeTower(x, y)) {
                std::cout << "Башня размещена в (" << x << "," << y << ")\n";
                return;
            }
        }
    }
    
    std::cout << "Не удалось разместить башню - нет свободных клеток!\n";
}

std::vector<std::pair<int, int>> EnemyManager::findSpawnPositions(const GameField& field, int playerX, int playerY) {
    std::vector<std::pair<int, int>> positions;
    
    for (int y = 0; y < field.getHeight(); y++) {
        for (int x = 0; x < field.getWidth(); x++) {
            if (field.isCellEmpty(x, y)) {
                int dist = std::max(std::abs(x - playerX), std::abs(y - playerY));
                if (dist >= 5) {
                    positions.push_back({x, y});
                }
            }
        }
    }
    
    return positions;
}

void EnemyManager::reset() {
    enemiesDefeated = 0;
    totalEnemiesSpawned = 0;
    currentLevel = 1;
}

void EnemyManager::setEnemiesDefeated(int count) {
    enemiesDefeated = count;
}