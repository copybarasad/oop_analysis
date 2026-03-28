#include "enemy_building.h"
#include <stdexcept>

EnemyBuilding::EnemyBuilding(int cooldown) 
    : position(0, 0), spawnCooldown(5), currentCooldown(0) {
    if (cooldown <= 0) {
        throw std::invalid_argument("Cooldown must be positive");
    }
    validateCooldown();
}

void EnemyBuilding::validateCooldown() {
    if (currentCooldown < 0) {
        currentCooldown = 0;
    }
}

Position EnemyBuilding::getPosition() const {
    return position;
}

void EnemyBuilding::setPosition(const Position& newPosition) {
    position = newPosition;
}

bool EnemyBuilding::canSpawnEnemy() const {
    return currentCooldown == 0;
}

void EnemyBuilding::update() {
    if (currentCooldown > 0) {
        --currentCooldown;
    }
    validateCooldown();
}

void EnemyBuilding::resetCooldown() {
    currentCooldown = spawnCooldown;
}