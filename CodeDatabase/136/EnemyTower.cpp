#include "EnemyTower.h"
#include <iostream>

EnemyTower::EnemyTower(int range, int damage, int cooldown, int health)
    : attackRange(range), damage(damage), attackCooldown(cooldown), 
      currentCooldown(0), isActive(true), health(health), maxHealth(health) {}

void EnemyTower::update() {
    if (currentCooldown > 0) {
        currentCooldown--;
    }
}

bool EnemyTower::canAttack() const {
    return isActive && currentCooldown == 0;
}

bool EnemyTower::isInRange(const Position& towerPos, const Position& targetPos) const {
    int distance = std::max(std::abs(targetPos.x - towerPos.x), 
                           std::abs(targetPos.y - towerPos.y));
    return distance <= attackRange;
}

void EnemyTower::performAttack(const Position& towerPos, const Position& playerPos, 
                              GameField& field, Wanderer& player) {
    if (!canAttack() || !isInRange(towerPos, playerPos)) {
        return;
    }
    
    if (!field.hasLineOfSight(towerPos.x, towerPos.y, playerPos.x, playerPos.y)) {
        return;
    }

    player.takeDamage(damage);
    std::cout << "Enemy tower attacks you for " << damage << " damage!\n";
    
    currentCooldown = attackCooldown;
}

void EnemyTower::takeDamage(int damage) {
    if (!isActive) return;
    health -= damage;
    if (health <= 0) {
        health = 0;
        isActive = false;
    }
}

bool EnemyTower::isDestroyed() const {
    return health <= 0;
}

bool EnemyTower::getIsActive() const {
    return isActive;
}

void EnemyTower::setActive(bool active) {
    isActive = active;
}

int EnemyTower::getAttackRange() const {
    return attackRange;
}

int EnemyTower::getHealth() const {
    return health;
}

int EnemyTower::getMaxHealth() const {
    return maxHealth;
}