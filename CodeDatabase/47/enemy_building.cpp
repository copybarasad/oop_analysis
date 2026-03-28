#include "enemy_building.h"

EnemyBuilding::EnemyBuilding(int startX, int startY) {
    positionX = startX;
    positionY = startY;
    health = BUILDING_HEALTH;
    spawnCooldown = DEFAULT_SPAWN_COOLDOWN;
    currentCooldown = 0;
}

int EnemyBuilding::getPositionX() const {
    return positionX;
}

int EnemyBuilding::getPositionY() const {
    return positionY;
}

int EnemyBuilding::getHealth() const {
    return health;
}

int EnemyBuilding::getCurrentCooldown() const {
    return currentCooldown;
}

int EnemyBuilding::getSpawnCooldown() const {
    return spawnCooldown;
}

void EnemyBuilding::takeDamage(int damageAmount) {
    health -= damageAmount;
    if (health <= 0) {
        health = 0;
    }
}

void EnemyBuilding::update() {
    if (isAlive()) {
        currentCooldown++;
    }
}

bool EnemyBuilding::canSpawnEnemy() const {
    return isAlive() && currentCooldown >= spawnCooldown;
}

void EnemyBuilding::resetCooldown() {
    currentCooldown = 0;
}

bool EnemyBuilding::isAlive() const {
    return health > 0;
}

void EnemyBuilding::setPosition(int x, int y) {
    positionX = x;
    positionY = y;
}

void EnemyBuilding::setHealth(int h) {
    health = h;
    if (health <= 0) {
        health = 0;
    }
}

void EnemyBuilding::setCooldown(int c) {
    currentCooldown = c;
}