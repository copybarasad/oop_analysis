#include "EnemyTower.h"
#include <algorithm>

EnemyTower::EnemyTower(int health, int damage, int x, int y, int attackRange)
    : health(health), damage(damage), attackRange(attackRange), position(x, y), attackCooldown(0) {}

bool EnemyTower::isAlive() const {
    return health > 0;
}

void EnemyTower::takeDamage(int amount) {
    health = std::max(0, health - amount);
}

int EnemyTower::getHealth() const { 
    return health;
}

int EnemyTower::getDamage() const {
    return damage;
}

std::pair<int, int> EnemyTower::getPosition() const {
    return position;
}

int EnemyTower::getAttackRange() const {
    return attackRange;
}

bool EnemyTower::canAttackThisTurn() const {
    return isAlive() && attackCooldown == 0;
}

void EnemyTower::markAttackUsed() {
    attackCooldown = 2;
}

void EnemyTower::update() {
    if (attackCooldown > 0) {
        attackCooldown--;
    }
}