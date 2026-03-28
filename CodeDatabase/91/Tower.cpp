#include "Tower.h"
#include "../Game.h"
#include "SaveLoad.h"
#include <iostream>

Tower::Tower(const Position& pos, int towerRange, int health)
    : position_(pos),
      range_(towerRange),
      cooldown_(2),
      currentCooldown_(0),
      active_(true),
      attack_spell_(15, towerRange, "Tower Bolt", 0),
      health_(health) {}

void Tower::update(Game& /*game*/) {
    if (currentCooldown_ > 0) {
        --currentCooldown_;
    }
}

bool Tower::canAttack(const Game& game) const {
    if (!active_ || currentCooldown_ > 0) {
        return false;
    }
    return game.isPlayerInRange(position_, range_);
}

void Tower::attack(Game& game) {
    if (!canAttack(game)) {
        return;
    }

    currentCooldown_ = cooldown_;

    Position playerPos = game.getPlayer().getPosition();
    bool success = attack_spell_.cast(game, playerPos);

    if (success) {
        std::cout << "Enemy Tower at (" << position_.x << ", " << position_.y
                  << ") casts " << attack_spell_.getName()
                  << " on you for " << attack_spell_.getDamage() << " damage!\n";
    } else {
        std::cout << "Enemy Tower's spell failed to hit you!\n";
    }
}

void Tower::takeDamage(int damage) {
    if (!active_ || damage <= 0) return;
    health_ -= damage;
    if (health_ <= 0) {
        health_ = 0;
        active_ = false;
        std::cout << "Enemy Tower destroyed!\n";
    } else {
        std::cout << "Enemy Tower takes " << damage << " damage. "
                  << health_ << " HP remaining.\n";
    }
}

bool Tower::isAlive() const {
  return active_ && health_ > 0;
}

int Tower::getHealth() const {
  return health_;
}

Position Tower::getPosition() const {
    return position_;
}

int Tower::getRange() const {
    return range_;
}

bool Tower::isActive() const {
    return active_;
}

int Tower::getCooldown() const {
    return currentCooldown_;
}

char Tower::getSymbol() const {
    return 'T';
}

void Tower::applyLevelBonus(int level) {
    int health_increase_per_level = 15;
    int damage_increase_per_level = 3;

    health_ += health_increase_per_level * level;

    attack_spell_.setDamage(attack_spell_.getDamage() + damage_increase_per_level * level);
}

void Tower::setActive(bool is_active) {
    active_ = is_active;
}

// === JSON ===
const DamageSpell& Tower::getAttackSpell() const {
    return attack_spell_;
}

void Tower::loadState(const nlohmann::json& j) {
    Position new_pos = j.at("position").get<Position>();
    position_ = new_pos;

    range_ = j.at("range").get<int>();
    currentCooldown_ = j.at("cooldown").get<int>();
    active_ = j.at("active").get<bool>();
    j.at("attack_spell").get_to(attack_spell_);
    health_ = j.at("health").get<int>();
}