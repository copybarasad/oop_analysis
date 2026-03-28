#include "EnemyBuilding.h"

EnemyBuilding::EnemyBuilding(int cooldown, int health)
    : spawnCooldown(cooldown), turnCounter(0), health_(health), maxHealth_(health) {}

bool EnemyBuilding::canSpawn() const {
    return turnCounter >= spawnCooldown;
}

void EnemyBuilding::nextTurn() { ++turnCounter; }
void EnemyBuilding::resetTurn() { turnCounter = 0; }

void EnemyBuilding::takeDamage(int amount) {
    if (amount > 0) {
        health_ -= amount;
        if (health_ < 0) health_ = 0;
    }
}