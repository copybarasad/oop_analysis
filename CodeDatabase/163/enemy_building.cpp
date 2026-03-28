#include "enemy_building.hpp"
#include "map.hpp"
#include "enemy.hpp"
#include "player.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

bool EnemyBuilding::randomInitialized = false;

void EnemyBuilding::initRandom() {
    if (!randomInitialized) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        randomInitialized = true;
    }
}

EnemyBuilding::EnemyBuilding(int startX, int startY, Cell& cell, int hp, int cooldown,
                             std::vector<std::unique_ptr<GameObject>>& enemyList, Player* player)
    : GameObject(startX, startY, cell, hp, 0),
      spawnCooldown(cooldown > 0 ? cooldown : SPAWN_COOLDOWN_DEFAULT),
      currentCooldown(0),
      enemies(enemyList),
      targetPlayer(player),
      hasSpawnedThisTurn(false) {
    
    initRandom();
    
    if (!player) {
        throw std::invalid_argument("Для вражеского здания нужна цель в виде игрока");
    }
}

void EnemyBuilding::takeTurn(Map& map) {
    if (!isAlive()) {
        return;
    }
    
    resetTurnFlags();
    
    if (currentCooldown > 0) {
        currentCooldown--;
    }
    
    if (canSpawn()) {
        if (trySpawnEnemy(map)) {
            currentCooldown = spawnCooldown;
            hasSpawnedThisTurn = true;
        }
    }
}

void EnemyBuilding::resetTurnFlags() {
    hasSpawnedThisTurn = false;
}

bool EnemyBuilding::canSpawn() const {
    return currentCooldown <= 0;
}

bool EnemyBuilding::isValidSpawnPosition(Map& map, int x, int y) const {
    if (!map.isValidPosition(x, y)) {
        return false;
    }
    
    const Cell& cell = map.getCell(x, y);
    
    if (!cell.isPassable() || !cell.isEmptyCell()) {
        return false;
    }
    
    int distanceToBuilding = std::abs(x - positionX) + std::abs(y - positionY);
    if (distanceToBuilding < 1) {
        return false;
    }
    
    return true;
}

void EnemyBuilding::findSpawnPosition(Map& map, int& outX, int& outY) const {
    int directions[4][2] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };
    
    initRandom();
    int startIndex = std::rand() % 4;
    
    for (int i = 0; i < 4; i++) {
        int index = (startIndex + i) % 4;
        int spawnX = positionX + directions[index][0];
        int spawnY = positionY + directions[index][1];
        
        if (isValidSpawnPosition(map, spawnX, spawnY)) {
            outX = spawnX;
            outY = spawnY;
            return;
        }
    }
    
    for (int dx = -2; dx <= 2; dx++) {
        for (int dy = -2; dy <= 2; dy++) {
            if (dx == 0 && dy == 0) continue;
            
            int spawnX = positionX + dx;
            int spawnY = positionY + dy;
            
            if (isValidSpawnPosition(map, spawnX, spawnY)) {
                outX = spawnX;
                outY = spawnY;
                return;
            }
        }
    }
    
    outX = -1;
    outY = -1;
}

bool EnemyBuilding::trySpawnEnemy(Map& map) {
    if (!canSpawn()) {
        return false;
    }
    
    int spawnX, spawnY;
    findSpawnPosition(map, spawnX, spawnY);
    
    if (spawnX == -1 || spawnY == -1) {
        std::cout << "Вражеское здание находится в клетке (" << positionX << ", " << positionY 
                  << ") Негде заспавнить\n";
        return false;
    }
    
    try {
        Cell& spawnCell = map.getCell(spawnX, spawnY);
        auto newEnemy = std::make_unique<Enemy>(
            spawnX, spawnY, spawnCell, 30, 2, 10, targetPlayer);
        
        enemies.push_back(std::move(newEnemy));
        
        std::cout << "Заспавнили врага в" 
                  << spawnX << ", " << spawnY << ")\n";
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Не получилось заспавнить: " << e.what() << "\n";
        return false;
    }
}