#ifndef OOP_ENEMY_TOWER_H_
#define OOP_ENEMY_TOWER_H_

#include "position.h"
#include <memory>

namespace rpg {
    class EnemyTower final {
    public:
        static constexpr int kDefaultAttackDamage = 15;
        static constexpr int kDefaultAttackRange = 4;
        static constexpr int kDefaultCooldown = 2;

        EnemyTower(const Position &position,
                   int attack_damage = kDefaultAttackDamage,
                   int attack_range = kDefaultAttackRange,
                   int cooldown = kDefaultCooldown);

        static EnemyTower FromSave(const Position &position, int cooldown, int turns_until_attack);

        [[nodiscard]] Position GetPosition() const;

        [[nodiscard]] int GetCooldown() const;

        [[nodiscard]] int GetTurnsUntilAttack() const;
        
        [[nodiscard]] int GetAttackDamage() const;
        
        [[nodiscard]] int GetAttackRange() const;
        
        void UpdateCooldown();
        
        [[nodiscard]] bool CanAttack(const Position& target_pos) const;
        
        void ResetCooldown();

    private:
        Position position_;
        int attack_damage_;
        int attack_range_;
        int cooldown_;
        int turns_until_attack_;

        [[nodiscard]] bool IsPlayerInRange(const Position &player_pos) const;
    };
}

#endif  // OOP_ENEMY_TOWER_H_
