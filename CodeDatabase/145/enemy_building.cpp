#include "enemy_building.h"
#include "enemy.h"
#include <cstdlib>
#include <cmath>

EnemyBuilding::EnemyBuilding(int x, int y, int spawnInterval) 
    : GameEntity(40, 0),  
      position(x, y),
      spawnCounter(0),
      spawnInterval(spawnInterval) {}

std::pair<int, int> EnemyBuilding::getPosition() const {
    return position;
}

void EnemyBuilding::setPosition(int x, int y) {
    position = {x, y};
}

bool EnemyBuilding::shouldSpawnEnemy() const {
    return spawnCounter >= spawnInterval;
}

void EnemyBuilding::update() {
    spawnCounter++;
}

Enemy EnemyBuilding::spawnEnemy() const {
    
    int offsetX = (rand() % 3) - 1;  
    int offsetY = (rand() % 3) - 1;  
    
    
    if (offsetX == 0 && offsetY == 0) {
        offsetX = 1;
    }
    
    return Enemy(15, 5, position.first + offsetX, position.second + offsetY);  // Уменьшено с 20 на 15
}

int EnemyBuilding::getScore() const {
    return 150;  
}

int EnemyBuilding::getSpawnInterval() const {
    return spawnInterval;
}

int EnemyBuilding::getSpawnCounter() const {
    return spawnCounter;
}

void EnemyBuilding::setSpawnCounter(int c) {
    spawnCounter = c;
}