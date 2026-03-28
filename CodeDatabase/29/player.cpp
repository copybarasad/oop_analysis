#include "player.hpp"

Player::Player(const std::string& name,
               int hp,
               int melee,
               int ranged,
               Point pos,
               std::size_t hand_capacity,
               int ranged_range)
    : Actor(hp, melee, pos),
      name_(name),
      melee_damage_(melee),
      ranged_damage_(ranged),
      mode_(CombatMode::kMelee),
      score_(0),
      hand_(hand_capacity),
      ranged_range_(ranged_range),
      max_hp_(hp)
{
    if (ranged_range_ <= 0) ranged_range_ = 1;
}

const std::string& Player::Name() const noexcept {
    return name_;
}

CombatMode Player::Mode() const noexcept {
    return mode_;
}

bool Player::IsMeleeMode() const noexcept {
    return mode_ == CombatMode::kMelee;
}

void Player::ToggleMode() noexcept {
    mode_ = (mode_ == CombatMode::kMelee ? CombatMode::kRanged : CombatMode::kMelee);
}

int Player::CurrentDamage() const noexcept {
    return mode_ == CombatMode::kMelee ? melee_damage_ : ranged_damage_;
}

int Player::RangedRange() const noexcept {
    return ranged_range_;
}

int Player::MeleeRange() const noexcept {
    return 1;
}

int Player::Score() const noexcept {
    return score_;
}

int Player::MaxHP() const noexcept {
    return max_hp_;
}

int Player::MeleeDamage() const noexcept {
    return melee_damage_;
}

int Player::RangedDamage() const noexcept {
    return ranged_damage_;
}

void Player::AddScore(int s) noexcept {
    if (s > 0) score_ += s;
}

bool Player::SpendScore(int s) noexcept {
    if (s <= 0) return false;
    if (score_ < s) return false;
    score_ -= s;
    return true;
}

PlayerHand& Player::Hand() noexcept {
    return hand_;
}

const PlayerHand& Player::Hand() const noexcept {
    return hand_;
}

void Player::RestoreHealthToFull() noexcept {
    hp_ = max_hp_;
}

void Player::IncreaseMaxHealth(int delta) {
    if (delta <= 0) {
        return;
    }
    max_hp_ += delta;
    hp_ += delta;
}

void Player::IncreaseMeleeDamage(int delta) {
    melee_damage_ += delta;
    if (melee_damage_ < 0) {
        melee_damage_ = 0;
    }
}

void Player::IncreaseRangedDamage(int delta) {
    ranged_damage_ += delta;
    if (ranged_damage_ < 0) {
        ranged_damage_ = 0;
    }
}

void Player::SetScore(int value) noexcept {
    if (value < 0) value = 0;
    score_ = value;
}

void Player::SetMode(CombatMode mode) noexcept {
    mode_ = mode;
}

void Player::SetHP(int value) {
    if (value < 0) value = 0;
    if (value > max_hp_) value = max_hp_;
    hp_ = value;
}

void Player::SetMaxHP(int value) {
    if (value < 1) value = 1;
    max_hp_ = value;
    if (hp_ > max_hp_) hp_ = max_hp_;
}

void Player::SetRangedRange(int value) {
    if (value < 1) value = 1;
    ranged_range_ = value;
}
