#include "Enemy.h"
#include <stdexcept>

Enemy::Enemy(int startX, int startY, int enemyHealth, int enemyDamage, int maxX, int maxY)
    : Character(startX, startY, enemyHealth, maxX, maxY), damage(enemyDamage) {
    if (damage <= 0) {
        throw std::invalid_argument("Enemy damage must be positive");
    }
}

int Enemy::getDamage() const {
    return damage;
}
