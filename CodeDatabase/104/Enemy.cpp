#include "Enemy.h"

Enemy::Enemy(int health, int damage)
    : health_(health), damage_(damage), position_x_(0), position_y_(0) {
}

int Enemy::GetHealth() const {
    return health_;
}

int Enemy::GetDamage() const {
    return damage_;
}

void Enemy::SetPosition(int x, int y) {
    position_x_ = x;
    position_y_ = y;
}

void Enemy::GetPosition(int& x, int& y) const {
    x = position_x_;
    y = position_y_;
}

void Enemy::TakeDamage(int damage) {
    health_ -= damage;
    if (health_ < 0) health_ = 0;
}

bool Enemy::IsAlive() const {
    return health_ > 0;
}