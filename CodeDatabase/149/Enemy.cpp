#include "Enemy.h"

Enemy::Enemy(int initialHealth, int enemyDamage) : health(initialHealth), damage(enemyDamage) {}

void Enemy::takeDamage(int damageTaken) {
    health -= damageTaken;
    if (health < 0) {
        health = 0;
    }
}

bool Enemy::isAlive() const {
    return health > 0;
}

int Enemy::getDamage() const {
    return damage;
}

int Enemy::getHealth() const {
    return health;
}

bool Enemy::isBuilding() const {
    return false;
}