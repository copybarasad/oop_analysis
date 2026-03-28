#include "Player.h"
#include <algorithm>

Player::Player(int health, int melee_damage, int ranged_damage, int score, int x, int y)
        : Creature(health, 0, x, y),
          score_(score),
          melee_damage_(melee_damage),
          ranged_damage_(ranged_damage),
          max_health_(health),
          combat_mode_(CombatMode::kMelee),
          is_slowed_(false),
          hand_(5),
          boost_charges_(0) {}

Player::Player() : Player(100, 20, 10, 0, 0, 0) {}

int Player::getDamage() const {
    return combat_mode_ == CombatMode::kMelee ? melee_damage_ : ranged_damage_;
}

void Player::switchCombatMode() {
    combat_mode_ = (combat_mode_ == CombatMode::kMelee) ? CombatMode::kRanged : CombatMode::kMelee;
}

int Player::getScore() const {
    return score_;
}

void Player::addScore(int points) {
    score_ += points;
}

std::string Player::getModeName() const {
    return combat_mode_ == CombatMode::kMelee ? "Melee" : "Ranged";
}

bool Player::isSlowed() const {
    return is_slowed_;
}

void Player::setSlowed(bool slowed) {
    is_slowed_ = slowed;
}

PlayerHand& Player::getHand() {
    return hand_;
}

void Player::addBoostCharge(int amount) {
    boost_charges_ += amount;
}

int Player::consumeBoostCharges() {
    int charges = boost_charges_;
    boost_charges_ = 0;
    return charges;
}


void Player::heal() {
    health_ = max_health_;
}

void Player::setMaxHealth(int new_max) {
    max_health_ = new_max;
    if (health_ > max_health_) health_ = max_health_;
}

void Player::increaseDamage(int amount) {
    melee_damage_ += amount;
    ranged_damage_ += amount;
}

int Player::getBoostCharges() const {
    return boost_charges_;
}

void Player::setBoostCharges(int amount) {
    boost_charges_ = amount;
}