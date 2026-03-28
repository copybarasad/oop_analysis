#include "CombatManager.h"
#include "GameVisualize.h"

#include <iostream>
#include <ctime>

CombatManager::CombatManager(std::vector<Enemy>& enemiesRef, 
                             std::vector<std::pair<int, int>>& positionsRef,
                             std::vector<std::vector<Cell>>& gridRef,
                             int w, int h)
    : enemies(enemiesRef), enemyPositions(positionsRef), grid(gridRef), width(w), height(h) {}


bool CombatManager::attackEnemy(int x, int y, Player& player) {
    for (size_t i = 0; i < enemyPositions.size(); i++) {

        if (enemyPositions[i].first == x && enemyPositions[i].second == y && enemies[i].isAlive()) {
            int damage = player.getDamage();
            enemies[i].takeDamage(damage);
            
            visualizer.visualizePlayerAttack(damage, 
                                            enemies[i].getHealth(), 
                                            enemies[i].getMaxHealth());
            
            if (!enemies[i].isAlive()) {
                player.addScore(10);
                visualizer.visualizeEnemyDefeated();
            }
            return true;
        }
    }
    return false;
}

void CombatManager::attackPlayer(int enemyIndex, Player& player) {
    int damage = enemies[enemyIndex].getDamage();
    player.takeDamage(damage);
    
    // Используем более информативный метод
    visualizer.visualizeEnemyAttack(enemies[enemyIndex], damage);
}

void CombatManager::moveEnemies(Player& player) {
    std::mt19937 gen(std::time(nullptr));
    std::uniform_int_distribution<> dirDist(0, 3);
    
    for (size_t i = 0; i < enemyPositions.size(); i++) {

        if (!enemies[i].isAlive()) continue;
        
        int x = enemyPositions[i].first;
        int y = enemyPositions[i].second;
        int direction = dirDist(gen);
        
        int newX = x, newY = y;
        switch (direction) {
            case 0: newY--; break;
            case 1: newX++; break;
            case 2: newY++; break;
            case 3: newX--; break;
        }
        
        if (isValidPosition(newX, newY)) {

            if (hasPlayer(newX, newY)) {
                attackPlayer(i, player);

            } else if (isCellPassable(newX, newY) && !hasEnemy(newX, newY) && !hasBuilding(newX, newY)) {
                grid[y][x].setEnemyPresence(false);
                grid[newY][newX].setEnemyPresence(true);
                enemyPositions[i] = {newX, newY};
            }
        }
    }
}

void CombatManager::removeDeadEnemies() {
    for (size_t i = 0; i < enemies.size(); ) {
        if (!enemies[i].isAlive()) {
            auto pos = enemyPositions[i];
            grid[pos.second][pos.first].setEnemyPresence(false);
            enemyPositions.erase(enemyPositions.begin() + i);
            enemies.erase(enemies.begin() + i);

        } else {
            i++;
        }
    }
}

bool CombatManager::isValidPosition(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

bool CombatManager::isCellPassable(int x, int y) const {
    return isValidPosition(x, y) && grid[y][x].isPassable();
}

bool CombatManager::hasPlayer(int x, int y) const {
    return isValidPosition(x, y) && grid[y][x].hasPlayerHere();
}

bool CombatManager::hasEnemy(int x, int y) const {
    return isValidPosition(x, y) && grid[y][x].hasEnemyHere();
}

bool CombatManager::hasBuilding(int x, int y) const {
    return isValidPosition(x, y) && grid[y][x].hasBuildingHere();
}