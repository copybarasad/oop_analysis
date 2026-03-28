#include "EnemyBuilding.h"

EnemyBuilding::EnemyBuilding(int health, int spawnInterval, const std::string& name)
    : GameObject(health, name), spawnCounter(0), spawnInterval(spawnInterval) {}

bool EnemyBuilding::shouldSpawnEnemy() const {
    return spawnCounter >= spawnInterval;
}

void EnemyBuilding::update() {
    spawnCounter++;
}

void EnemyBuilding::resetCounter() {
    spawnCounter = 0;
}

std::string EnemyBuilding::getType() const {
    return "EnemyBuilding";
}
