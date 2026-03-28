#include "Enemy.h"

Enemy::Enemy(int start_x, int start_y, int health, int damage)
    : health(health), damage(damage), position(start_x, start_y) {
}

void Enemy::take_Damage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

bool Enemy::isAlive() const {
    return health > 0;
}

int Enemy::get_Health() const {
    return health;
}

int Enemy::get_Damage() const {
    return damage;
}

const Position& Enemy::get_Position() const {
    return position;
}

void Enemy::set_Position(const Position& newPosition) {
    position = newPosition;
}

void Enemy::setHealth(int h) {
    health = h;
}