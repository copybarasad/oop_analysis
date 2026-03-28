#include "EnemyTower.h"
#include <cmath>

EnemyTower::EnemyTower(Pos pos, int range, int damage, int cooldown, int health)
    : position_(pos), attackRange_(range), attackDamage_(damage), 
      attackCooldown_(cooldown), cooldownCounter_(0), health_(health), maxHealth_(health) {}

void EnemyTower::takeDamage(int amount) {
    if (amount > 0) {
        health_ -= amount;
        if (health_ < 0) health_ = 0;
    }
}

bool EnemyTower::isInRange(const Pos& target) const {
    int distance = std::abs(position_.x - target.x) + std::abs(position_.y - target.y);
    return distance <= attackRange_;
}

