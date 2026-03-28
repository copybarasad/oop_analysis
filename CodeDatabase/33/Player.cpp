#include "Player.h"

Player::Player()
    : name_("Player"),
      mode_(Mode::Melee),
      melee_(0),
      ranged_(0),
      score_(0) {}

Player::Player(std::string name, int hp, int melee, int ranged, Coord pos)
    : Character(hp, melee, pos),
      name_(std::move(name)),
      mode_(Mode::Melee),
      melee_(melee),
      ranged_(ranged),
      score_(0),
      base_hp_(hp) {}

const std::string& Player::Name() const { return name_; }
void Player::SetName(const std::string& n) { if (!n.empty()) name_ = n; }

Player::Mode Player::CurrentMode() const { return mode_; }
void Player::ToggleMode() {
    mode_ = (mode_ == Mode::Melee) ? Mode::Ranged : Mode::Melee;
}

int Player::MeleeDamage() const { return melee_; }
void Player::SetMelee(int v) { melee_ = v; }
void Player::SetRanged(int v) { ranged_ = v; }
int Player::RangedDamage() const { return ranged_; }
int Player::CurrentDamage() const {
    return (mode_ == Mode::Melee) ? melee_ : ranged_;
}

int Player::Score() const { return score_; }
void Player::SetScore(int s) { score_ = s; }
void Player::AddScore(int s) { if (s > 0) score_ += s; }

int  Player::BaseHP() const { return base_hp_; }
void Player::SetBaseHP(int v) { base_hp_ = v; }
void Player::HealFull() { SetHP(base_hp_); } 