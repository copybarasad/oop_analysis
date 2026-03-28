#include "EnemyTower.h"
#include <algorithm>

EnemyTower::EnemyTower(int x, int y, int health, int damage, int range, int cooldown)
    : x_(x),
      y_(y),
      health_(health),
      max_health_(health),
      damage_(damage),
      range_(range),
      cooldown_(cooldown),
      current_cooldown_(0) {}

int EnemyTower::GetX() const {
    return x_;
}

int EnemyTower::GetY() const {
    return y_;
}

int EnemyTower::GetHealth() const {
    return health_;
}

int EnemyTower::GetMaxHealth() const {
    return max_health_;
}

int EnemyTower::GetDamage() const {
    return damage_;
}

int EnemyTower::GetRange() const {
    return range_;
}

int EnemyTower::GetCooldown() const {
    return cooldown_;
}

int EnemyTower::GetCurrentCooldown() const {
    return current_cooldown_;
}

bool EnemyTower::CanAttack() const {
    return current_cooldown_ == 0;
}

void EnemyTower::Attack() {
    current_cooldown_ = cooldown_;
}

void EnemyTower::UpdateCooldown() {
    if (current_cooldown_ > 0) {
        current_cooldown_--;
    }
}

void EnemyTower::SetCurrentCooldown(int cooldown) {
    current_cooldown_ = (cooldown > 0) ? cooldown : 0;
}

void EnemyTower::TakeDamage(int damage) {
    health_ = std::max(0, health_ - damage);
}

bool EnemyTower::IsAlive() const {
    return health_ > 0;
}

void EnemyTower::IncreaseStats(int health_bonus, int damage_bonus) {
    max_health_ += health_bonus;
    health_ += health_bonus;
    damage_ += damage_bonus;
}
