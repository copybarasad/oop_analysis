#include "enemy.h"
#include <stdexcept>

Enemy::Enemy(int enemyHealth, int enemyDamage) 
    : health(enemyHealth), damage(enemyDamage), position(0, 0) { 
    validateHealth();
}

void Enemy::validateHealth() {
    if (health <= 0) {
        health = 0;
    }
}

void Enemy::setHealth(int newHealth) {
    if (newHealth < 0) {
        throw std::invalid_argument("Health cannot be negative");
    }
    health = newHealth;
    validateHealth();
}

void Enemy::setDamage(int newDamage) {
    if (newDamage < 0) {
        throw std::invalid_argument("Damage cannot be negative");
    }
    damage = newDamage;
}

int Enemy::getHealth() const {
    return health;
}

int Enemy::getDamage() const {
    return damage;
}

Position Enemy::getPosition() const {
    return position;
}

void Enemy::setPosition(const Position& newPosition) {
    position = newPosition;
}

void Enemy::takeDamage(int damageAmount) {
    if (damageAmount < 0) {
        throw std::invalid_argument("Damage cannot be negative");
    }
    health -= damageAmount;
    validateHealth();
}

bool Enemy::isAlive() const {
    return health > 0;
}