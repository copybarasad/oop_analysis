#include "character.h"
#include <algorithm>

Character::Character(int health, int damage)
    : health_(std::max(1, health)), max_health_(std::max(1, health)), damage_(std::max(0, damage)) {}

int Character::Health() const { return health_; }
int Character::MaxHealth() const { return max_health_; }
int Character::Damage() const { return damage_; }
bool Character::IsAlive() const { return health_ > 0; }

void Character::TakeDamage(int amount) {
    if (amount <= 0) return;
    health_ -= amount;
    if (health_ < 0) health_ = 0;
}

void Character::SetMaxHealth(int max_health) {
    max_health_ = std::max(1, max_health);
    if (health_ > max_health_) health_ = max_health_;
}

void Character::SetHealth(int health) {
    health_ = std::clamp(health, 0, max_health_);
}

void Character::SetDamage(int damage) {
    damage_ = damage;
}

void Character::IncreaseDamage(int amount) {
    damage_ += amount;
}

