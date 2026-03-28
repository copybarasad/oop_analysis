#include "Enemy.h"

Enemy::Enemy(int health, int damage)
    : health(health), damage(damage) {}

Enemy::Enemy(const Enemy& other)
    : health(other.health), damage(other.damage) {}

int Enemy::getHealth() const {
    return health;
}

int Enemy::getDamage() const {
    return damage;
}

void Enemy::takeDamage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

bool Enemy::isAlive() const {
    return health > 0;
}

void Enemy::setDamage(int newDamage) {
    damage = newDamage;
}
