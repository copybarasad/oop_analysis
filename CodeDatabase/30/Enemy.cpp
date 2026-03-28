#include <algorithm>
#include <iostream>
#include "include/Enemy.h"

Enemy::Enemy(int hp, int dmg) : damage_(dmg) { hp_ = std::max(0, hp); }

int Enemy::hp() const { return hp_; }

int Enemy::damage() const { return damage_; }

EntityType Enemy::type() const { return EntityType::Enemy; }

void Enemy::save(std::ostream& os) const {
    Entity::save(os);
    os << damage_ << "\n";
}

void Enemy::load(std::istream& is) {
    Entity::load(is);
    is >> damage_;
}
