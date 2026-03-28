#include "Enemy.h"
#include <algorithm>

Enemy::Enemy(int x, int y, int health, int damage)
    : health_(health),
      max_health_(health),
      damage_(damage),
      x_(x),
      y_(y),
      score_value_(10) {}

int Enemy::GetHealth() const {
    return health_;
}

int Enemy::GetMaxHealth() const {
    return max_health_;
}

int Enemy::GetDamage() const {
    return damage_;
}

int Enemy::GetX() const {
    return x_;
}

int Enemy::GetY() const {
    return y_;
}

int Enemy::GetScoreValue() const {
    return score_value_;
}

void Enemy::TakeDamage(int damage) {
    health_ = std::max(0, health_ - damage);
}

void Enemy::SetPosition(int x, int y) {
    x_ = x;
    y_ = y;
}

bool Enemy::IsAlive() const {
    return health_ > 0;
}

void Enemy::IncreaseStats(int health_bonus, int damage_bonus) {
    max_health_ += health_bonus;
    health_ += health_bonus;
    damage_ += damage_bonus;
    score_value_ += 5;
}
