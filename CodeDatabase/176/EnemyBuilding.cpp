#include "EnemyBuilding.h"
#include "Enemy.h"
#include <memory>

EnemyBuilding::EnemyBuilding(int interval, int health, int damage)
    : spawnCounter(0), spawnInterval(interval), enemyHealth(health),
      enemyDamage(damage) {}

bool EnemyBuilding::canSpawnEnemy() const {
    return spawnCounter >= spawnInterval;
}

void EnemyBuilding::update() {
    spawnCounter++;
}

std::unique_ptr<Enemy> EnemyBuilding::spawnEnemy() {
    if (canSpawnEnemy()) {
        spawnCounter = 0;
        return std::make_unique<Enemy>(enemyHealth, enemyDamage);
    }
    return nullptr;
}
