#include "../include/EnemyBuilding.h"

EnemyBuilding::EnemyBuilding(int spawnInterval, int health)
    : turnsUntilSpawn(spawnInterval), spawnInterval(spawnInterval), health(health), maxHealth(health) {}

int EnemyBuilding::getTurnsUntilSpawn() const {
    return turnsUntilSpawn;
}

int EnemyBuilding::getSpawnInterval() const {
    return spawnInterval;
}

void EnemyBuilding::setTurnsUntilSpawn(int turns) {
    turnsUntilSpawn = turns;
}

void EnemyBuilding::processTurn() {
    turnsUntilSpawn--;
    if (turnsUntilSpawn < 0) {
        turnsUntilSpawn = 0;
    }
}

void EnemyBuilding::resetSpawnTimer() {
    turnsUntilSpawn = spawnInterval;
}

bool EnemyBuilding::shouldSpawn() const {
    return turnsUntilSpawn == 0;
}

int EnemyBuilding::getHealth() const {
    return health;
}

int EnemyBuilding::getMaxHealth() const {
    return maxHealth;
}

int EnemyBuilding::takeDamage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
    return health;
}

bool EnemyBuilding::isAlive() const {
    return health > 0;
}
