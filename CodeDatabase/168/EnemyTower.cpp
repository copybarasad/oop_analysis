#include "EnemyTower.h"
#include <cmath>

EnemyTower::EnemyTower(int x, int y, int health, int damage, int attackRange, int maxCooldown)
    : position(x, y), health(health), damage(damage), attackRange(attackRange),
      cooldown(0), maxCooldown(maxCooldown) {
}

void EnemyTower::take_Damage(int dmg) {
    health -= dmg;
    if (health < 0) {
        health = 0;
    }
}

bool EnemyTower::isAlive() const {
    return health > 0;
}

int EnemyTower::get_Health() const {
    return health;
}

int EnemyTower::get_Damage() const {
    return damage;
}

int EnemyTower::get_AttackRange() const {
    return attackRange;
}

const Position& EnemyTower::get_Position() const {
    return position;
}

bool EnemyTower::canAttack() const {
    return cooldown == 0;
}

bool EnemyTower::isInRange(const Position& target) const {
    int dx = std::abs(target.x - position.x);
    int dy = std::abs(target.y - position.y);
    return dx <= attackRange && dy <= attackRange;
}

void EnemyTower::performAttack() {
    cooldown = maxCooldown;
}

void EnemyTower::updateCooldown() {
    if (cooldown > 0) {
        cooldown--;
    }
}

int EnemyTower::get_Cooldown() const {
    return cooldown;
}

int EnemyTower::get_MaxCooldown() const {
    return maxCooldown;
}

void EnemyTower::setHealth(int h) {
    health = h;
}

void EnemyTower::setCooldown(int cd) {
    cooldown = cd;
}







