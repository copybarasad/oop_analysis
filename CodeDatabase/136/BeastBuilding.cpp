#include "BeastBuilding.h"

BeastBuilding::BeastBuilding(int interval, int health)
    : spawnCounter(0), spawnInterval(interval), isActive(true),
      health(health), maxHealth(health) {}
    
bool BeastBuilding::shouldSpawnEnemy() const {
    return spawnCounter >= spawnInterval;
}

void BeastBuilding::update() {
    if (isActive) {
        spawnCounter++;
    }
}

void BeastBuilding::takeDamage(int damage) {
    if (!isActive) return;

    health -= damage;
    if (health <= 0) {
        health = 0;
        isActive = false;
    }
}

void BeastBuilding::setActive(bool active) {
    isActive = active;
}

bool BeastBuilding::isDestroyed() const {
    return health <= 0;
}

bool BeastBuilding::getIsActive() const {
    return isActive;
}

int BeastBuilding::getHealth() const {
    return health;
}

int BeastBuilding::getMaxHealth() const {
    return maxHealth;
}