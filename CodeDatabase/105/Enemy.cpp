#include "Enemy.h"
#include <algorithm>
#include <iostream>

Enemy::Enemy(int hp, int dmg) : health(hp), maxHealth(hp), damage(dmg) {}

int Enemy::getHealth() const { return health; }
int Enemy::getDamage() const { return damage; }
int Enemy::getMaxHealth() const { return maxHealth; }

void Enemy::takeDamage(int dmg) {
    health = std::max(0, health - dmg);
}

bool Enemy::isAlive() const {
    return health > 0;
}

void Enemy::setHealth(int hp) {
    health = std::min(hp, maxHealth);
}

void Enemy::setDamage(int dmg) {
    damage = dmg;
}