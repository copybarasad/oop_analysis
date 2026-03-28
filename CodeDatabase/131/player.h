#ifndef OOP_PLAYER_H_
#define OOP_PLAYER_H_

#include "position.h"
#include "spell_hand.h"

namespace rpg {
    enum class CombatMode {
        kMelee,
        kRanged
    };

    enum class UpgradeType {
        kIncreaseMaxHealth,
        kIncreaseMeleeDamage,
        kIncreaseRangedDamage
    };


    class Player final {
    public:
        Player(int max_health, int melee_damage, int ranged_damage,
               const Position &position);

        Player(int health, int max_health, int melee_damage, int ranged_damage,
               int score, const Position &position, CombatMode combat_mode,
               int slowed_turns);

        [[nodiscard]] int GetHealth() const;

        [[nodiscard]] Position GetPosition() const;

        [[nodiscard]] bool IsAlive() const;

        void TakeDamage(int damage);

        void SetPosition(const Position &pos);

        [[nodiscard]] int GetMaxHealth() const;

        [[nodiscard]] int GetDamage() const;

        [[nodiscard]] CombatMode GetCombatMode() const;

        [[nodiscard]] int GetScore() const;

        [[nodiscard]] bool CanMove() const;

        void Heal(int amount);

        void SwitchCombatMode();

        void AddScore(int points);

        void Slow();

        void UpdateAfterTurn();

        [[nodiscard]] SpellHand &GetSpellHand();

        [[nodiscard]] const SpellHand &GetSpellHand() const;

        void ApplyUpgrade(UpgradeType type, int value);

        [[nodiscard]] int GetMeleeDamage() const;

        [[nodiscard]] int GetRangedDamage() const;

    private:
        void IncreaseMaxHealth(int amount);

        void IncreaseMeleeDamage(int amount);

        void IncreaseRangedDamage(int amount);
        int health_;
        int max_health_;
        int melee_damage_;
        int ranged_damage_;
        int score_;
        Position position_;
        CombatMode combat_mode_;
        int slowed_turns_remaining_;
        SpellHand spell_hand_;

        // Инвариант: health_ >= 0 && health_ <= max_health_
        // Инвариант: melee_damage_ >= 0 && ranged_damage_ >= 0
        // Инвариант: score_ >= 0
        // Инвариант: slowed_turns_remaining_ >= 0
    };
}

#endif
