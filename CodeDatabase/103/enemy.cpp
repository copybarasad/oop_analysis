#include "enemy.h"

Enemy::Enemy(int startHealth, int startDamage)
    : Entity(startHealth, startDamage) {
    if (startHealth > 1000) {
        throw std::invalid_argument("Enemy health too high");
    }
    if (startHealth < 1) {
        throw std::invalid_argument("Enemy health too low");
    }
    if (startDamage < 1) {
        throw std::invalid_argument("Enemy damage too low");
    }
    }

void Enemy::takeDamage(int damage) {
    Entity::takeDamage(damage);
}