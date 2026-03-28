#include "player.h"
#include <algorithm>

namespace rpg {
    Player::Player(int max_health, int melee_damage, int ranged_damage,
                   const Position &position)
        : health_(max_health),
          max_health_(max_health),
          melee_damage_(std::max(0, melee_damage)),
          ranged_damage_(std::max(0, ranged_damage)),
          score_(0),
          position_(position),
          combat_mode_(CombatMode::kMelee),
          slowed_turns_remaining_(0),
          spell_hand_() {
        spell_hand_.InitializeWithRandomSpell();
    }


    Player::Player(int health, int max_health, int melee_damage, int ranged_damage,
                   int score, const Position &position, CombatMode combat_mode,
                   int slowed_turns)
        : health_(std::clamp(health, 0, std::max(0, max_health))),
          max_health_(std::max(0, max_health)),
          melee_damage_(std::max(0, melee_damage)),
          ranged_damage_(std::max(0, ranged_damage)),
          score_(std::max(0, score)),
          position_(position),
          combat_mode_(combat_mode),
          slowed_turns_remaining_(std::max(0, slowed_turns)),
          spell_hand_() {
    }

    [[nodiscard]] int Player::GetHealth() const {
        return health_;
    }

    [[nodiscard]] int Player::GetMaxHealth() const {
        return max_health_;
    }

    [[nodiscard]] int Player::GetDamage() const {
        return combat_mode_ == CombatMode::kMelee ? melee_damage_ : ranged_damage_;
    }

    [[nodiscard]] CombatMode Player::GetCombatMode() const {
        return combat_mode_;
    }

    [[nodiscard]] Position Player::GetPosition() const {
        return position_;
    }

    [[nodiscard]] int Player::GetScore() const {
        return score_;
    }

    [[nodiscard]] bool Player::IsAlive() const {
        return health_ > 0;
    }

    [[nodiscard]] bool Player::CanMove() const {
        return slowed_turns_remaining_ == 0;
    }

    void Player::TakeDamage(const int damage) {
        health_ = std::max(0, health_ - damage);
    }

    void Player::Heal(const int amount) {
        health_ = std::min(max_health_, health_ + amount);
    }

    void Player::SwitchCombatMode() {
        combat_mode_ = (combat_mode_ == CombatMode::kMelee)
                           ? CombatMode::kRanged
                           : CombatMode::kMelee;
    }

    void Player::SetPosition(const Position &pos) {
        position_ = pos;
    }

    void Player::AddScore(const int points) {
        score_ = std::max(0, score_ + points);
    }

    void Player::Slow() {
        slowed_turns_remaining_ = 2;
    }

    void Player::UpdateAfterTurn() {
        if (slowed_turns_remaining_ > 0) {
            slowed_turns_remaining_--;
        }
    }

    [[nodiscard]] SpellHand &Player::GetSpellHand() {
        return spell_hand_;
    }

    [[nodiscard]] const SpellHand &Player::GetSpellHand() const {
        return spell_hand_;
    }

    void Player::IncreaseMaxHealth(int amount) {
        max_health_ += amount;
        health_ += amount;
    }

    void Player::IncreaseMeleeDamage(int amount) {
        melee_damage_ += amount;
    }

    void Player::IncreaseRangedDamage(int amount) {
        ranged_damage_ += amount;
    }

    [[nodiscard]] int Player::GetMeleeDamage() const {
        return melee_damage_;
    }

    [[nodiscard]] int Player::GetRangedDamage() const {
        return ranged_damage_;
    }

    void Player::ApplyUpgrade(UpgradeType type, int value) {
        switch (type) {
            case UpgradeType::kIncreaseMaxHealth:
                IncreaseMaxHealth(value);
                break;
            case UpgradeType::kIncreaseMeleeDamage:
                IncreaseMeleeDamage(value);
                break;
            case UpgradeType::kIncreaseRangedDamage:
                IncreaseRangedDamage(value);
                break;
        }
    }
}
