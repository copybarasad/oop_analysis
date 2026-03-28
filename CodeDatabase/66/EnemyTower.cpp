#include "../include/EnemyTower.h"

EnemyTower::EnemyTower(int damage, int range, int cooldown, int health) 
    : damage(damage), range(range), cooldown(cooldown), currentCooldown(0), health(health), maxHealth(health) {}

int EnemyTower::getDamage() const {
    return damage;
}

int EnemyTower::getRange() const {
    return range;
}

bool EnemyTower::canAttack() const {
    return currentCooldown == 0;
}

void EnemyTower::processTurn() {
    if (currentCooldown > 0) {
        currentCooldown--;
    }
}

void EnemyTower::attack() {
    currentCooldown = cooldown;
}

int EnemyTower::getHealth() const {
    return health;
}

int EnemyTower::getMaxHealth() const {
    return maxHealth;
}

int EnemyTower::takeDamage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
    return health;
}

bool EnemyTower::isAlive() const {
    return health > 0;
}

int EnemyTower::getCooldownRemaining() const {
    return currentCooldown;
}

void EnemyTower::setCurrentCooldown(int cooldown) {
    currentCooldown = cooldown;
}
