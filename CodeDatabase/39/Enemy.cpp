#include "Enemy.h"
#include <stdexcept>

Enemy::Enemy(int max_health, int damage)
    : max_health_(max_health),
      health_(max_health),
      damage_(damage) {
    if (max_health_ <= 0 || damage_ <= 0) {
        throw std::invalid_argument("Enemy parameters must be positive.");
    }
    EnsureInvariants();
}

int Enemy::GetHealth() const {
    return health_;
}

int Enemy::GetDamage() const {
    return damage_;
}

bool Enemy::IsAlive() const {
    return health_ > 0;
}

void Enemy::TakeDamage(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Damage must be non-negative.");
    }
    health_ -= amount;
    if (health_ < 0) {
        health_ = 0;
    }
    EnsureInvariants();
}

void Enemy::EnsureInvariants() const {
    if (health_ < 0 || health_ > max_health_) {
        throw std::logic_error("Enemy health invariant violated.");
    }
    if (damage_ <= 0) {
        throw std::logic_error("Enemy damage invariant violated.");
    }
}
