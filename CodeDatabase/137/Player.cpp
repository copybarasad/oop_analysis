#include "Player.h"
#include <algorithm>

Player::Player(int x, int y, int max_health, int melee_damage, int ranged_damage)
    : health_(max_health),
      max_health_(max_health),
      melee_damage_(melee_damage),
      ranged_damage_(ranged_damage),
      score_(0),
      x_(x),
      y_(y),
      combat_mode_(CombatMode::MELEE),
      is_slowed_(false),
      experience_points_(0),
      level_(1) {}

int Player::GetHealth() const {
    return health_;
}

int Player::GetMaxHealth() const {
    return max_health_;
}

int Player::GetDamage() const {
    return combat_mode_ == CombatMode::MELEE ? melee_damage_ : ranged_damage_;
}

int Player::GetMeleeDamage() const {
    return melee_damage_;
}

int Player::GetRangedDamage() const {
    return ranged_damage_;
}

int Player::GetScore() const {
    return score_;
}

int Player::GetX() const {
    return x_;
}

int Player::GetY() const {
    return y_;
}

CombatMode Player::GetCombatMode() const {
    return combat_mode_;
}

bool Player::IsSlowed() const {
    return is_slowed_;
}

int Player::GetExperiencePoints() const {
    return experience_points_;
}

int Player::GetLevel() const {
    return level_;
}

void Player::TakeDamage(int damage) {
    health_ = std::max(0, health_ - damage);
}

void Player::Heal(int amount) {
    health_ = std::min(max_health_, health_ + amount);
}

void Player::AddScore(int points) {
    score_ += points;
}

void Player::SetPosition(int x, int y) {
    x_ = x;
    y_ = y;
}

void Player::SwitchCombatMode() {
    combat_mode_ = (combat_mode_ == CombatMode::MELEE) ? CombatMode::RANGED : CombatMode::MELEE;
}

void Player::SetSlowed(bool slowed) {
    is_slowed_ = slowed;
}

void Player::AddExperience(int exp) {
    experience_points_ += exp;
}

void Player::LevelUp() {
    level_++;
}

void Player::IncreaseMaxHealth(int amount) {
    max_health_ += amount;
    health_ += amount;
}

void Player::IncreaseDamage(int melee, int ranged) {
    melee_damage_ += melee;
    ranged_damage_ += ranged;
}

bool Player::IsAlive() const {
    return health_ > 0;
}

void Player::RestoreHealth() {
    health_ = max_health_;
}
