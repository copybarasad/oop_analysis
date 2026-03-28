#include "Player.h"
#include "Spell.h"
#include <stdexcept>

Player::Player(int max_health,
               int melee_damage,
               int ranged_damage,
               std::size_t hand_size)
    : max_health_(max_health),
      health_(max_health),
      melee_damage_(melee_damage),
      ranged_damage_(ranged_damage),
      score_(0),
      mode_(CombatMode::kMelee),
      ranged_range_(5),
      spell_hand_(hand_size) {
    EnsureInvariants();
}

SpellHand &Player::GetSpellHand() {
    return spell_hand_;
}

const SpellHand &Player::GetSpellHand() const {
    return spell_hand_;
}

bool Player::AddSpellToHand(std::unique_ptr<Spell> spell) {
    return spell_hand_.AddSpell(std::move(spell));
}

int Player::GetRange() const {
    return ranged_range_;
}

int Player::GetHealth() const {
    return health_;
}

int Player::GetMaxHealth() const {
    return max_health_;
}

int Player::GetScore() const {
    return score_;
}

Player::CombatMode Player::GetCombatMode() const {
    return mode_;
}

int Player::GetDamage() const {
    return mode_ == CombatMode::kMelee ? melee_damage_ : ranged_damage_;
}

bool Player::IsAlive() const {
    return health_ > 0;
}

void Player::TakeDamage(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Damage must be non-negative.");
    }
    health_ -= amount;
    if (health_ < 0) {
        health_ = 0;
    }
    EnsureInvariants();
}

void Player::AddScore(int amount) {
    if (amount < 0) {
        return;
    }
    score_ += amount;
}

void Player::SwitchMode() {
    if (mode_ == CombatMode::kMelee) {
        mode_ = CombatMode::kRanged;
    } else {
        mode_ = CombatMode::kMelee;
    }
}

void Player::EnsureInvariants() const {
    if (health_ < 0 || health_ > max_health_) {
        throw std::logic_error("Player health invariant violated.");
    }
    if (melee_damage_ <= 0 || ranged_damage_ <= 0) {
        throw std::logic_error("Player damage invariant violated.");
    }
}

int Player::GetPendingBuffStacks() const {
    return pending_buff_stacks_;
}

void Player::AddBuffStack(int amount) {
    if (amount <= 0) {
        return;
    }
    pending_buff_stacks_ += amount;
}

void Player::RestoreFullHealth() {
    health_ = max_health_;
}

int Player::ConsumeBuffStacks() {
    int result = pending_buff_stacks_;
    pending_buff_stacks_ = 0;
    return result;
}

void Player::IncreaseMaxHealth(int delta) {
    if (delta <= 0) return;
    max_health_ += delta;
}

void Player::IncreaseMeleeDamage(int delta) {
    if (delta <= 0) return;
    melee_damage_ += delta;
}

void Player::IncreaseRangedRange(int delta) {
    if (delta <= 0) return;
    ranged_range_ += delta;
}

void Player::IncreaseRangedDamage(int delta) {
    if (delta <= 0) return;
    ranged_damage_ += delta;
}
