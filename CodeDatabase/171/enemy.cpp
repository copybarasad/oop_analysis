#include "enemy.h"

Enemy::Enemy(int health, int damage, Position position)
    : health_(health), damage_(damage), position_(position) {
}

void Enemy::takeDamage(int damage) {
    health_ -= damage;
    if (health_ < 0) health_ = 0;
}

void Enemy::move(Position newPosition) {
    position_ = newPosition;
}

int Enemy::getHealth() const {
    return health_;
}

Position Enemy::getPosition() const {
    return position_;
}

int Enemy::getDamage() const {
    return damage_;
}

bool Enemy::isAlive() const {
    return health_ > 0;
}