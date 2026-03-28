#include "enemy.h"
#include <cassert>

Enemy::Enemy(int health, int damage, int x, int y) : health_(health), damage_(damage), x_(x), y_(y) {
    assert(health_ > 0 && "Health must be positive");
    assert(damage_ > 0 && "Damage must be positive");
}

int Enemy::GetHealth() const { return health_; }

int Enemy::GetDamage() const { return damage_; }

void Enemy::TakeDamage(int damage) { health_ -= damage; }

bool Enemy::IsAlive() const { return health_ > 0; }

int Enemy::GetX() const { return x_; }

int Enemy::GetY() const { return y_; }

void Enemy::SetPosition(int x, int y) { x_ = x; y_ = y; }

