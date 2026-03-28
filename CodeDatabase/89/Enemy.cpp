#include "Enemy.h"

Enemy::Enemy(int initialHealth, int initialDamage, int x, int y)
    : health(initialHealth), damage(initialDamage), position(x, y) {}

int Enemy::getHealth() const {
    return health;
}

int Enemy::getDamage() const { 
    return damage;
}

void Enemy::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

bool Enemy::isAlive() const {
    return health > 0;
}

std::pair<int, int> Enemy::getPosition() const {
    return position;
}

void Enemy::setPosition(int x, int y) {
    position.first = x;
    position.second = y;
}