#include "Enemy.h"

Enemy::Enemy(int enemyHealth, int enemyDamage) 
    : health(enemyHealth), damage(enemyDamage) {}

int Enemy::getHealth() const {
    return health;
}

int Enemy::getDamage() const {
    return damage;
}

bool Enemy::isAlive() const {
    return health > 0;
}

void Enemy::takeDamage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}
