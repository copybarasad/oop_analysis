#include "EnemyBuilding.h"

EnemyBuilding::EnemyBuilding(int interval) : spawnCounter(0), spawnInterval(interval) {}

bool EnemyBuilding::shouldSpawnEnemy() {
    spawnCounter++;
    if (spawnCounter >= spawnInterval) {
        spawnCounter = 0;
        return true;
    }
    return false;
}

int EnemyBuilding::getSpawnInterval() const { return spawnInterval; }
int EnemyBuilding::getSpawnCounter() const { return spawnCounter; }

void EnemyBuilding::setSpawnCounter(int counter) {
    spawnCounter = counter;
}