#include "enemy_tower.h"
#include <cmath>

namespace rpg {
    EnemyTower::EnemyTower(const Position &position,
                           int attack_damage,
                           int attack_range,
                           int cooldown)
        : position_(position),
          attack_damage_(attack_damage),
          attack_range_(attack_range),
          cooldown_(cooldown),
          turns_until_attack_(0) {
    }
    
    EnemyTower EnemyTower::FromSave(const Position &position, int cooldown, int turns_until_attack) {
        EnemyTower tower(position, kDefaultAttackDamage, kDefaultAttackRange, cooldown);
        tower.turns_until_attack_ = turns_until_attack;
        return tower;
    }

    [[nodiscard]] Position EnemyTower::GetPosition() const {
        return position_;
    }

    [[nodiscard]] int EnemyTower::GetCooldown() const {
        return cooldown_;
    }

    [[nodiscard]] int EnemyTower::GetTurnsUntilAttack() const {
        return turns_until_attack_;
    }
    
    [[nodiscard]] int EnemyTower::GetAttackDamage() const {
        return attack_damage_;
    }
    
    [[nodiscard]] int EnemyTower::GetAttackRange() const {
        return attack_range_;
    }
    
    void EnemyTower::UpdateCooldown() {
        if (turns_until_attack_ > 0) {
            --turns_until_attack_;
        }
    }
    
    [[nodiscard]] bool EnemyTower::CanAttack(const Position& target_pos) const {
        if (turns_until_attack_ > 0) {
            return false;
        }
        
        int dx = static_cast<int>(target_pos.x) - static_cast<int>(position_.x);
        int dy = static_cast<int>(target_pos.y) - static_cast<int>(position_.y);
        int distance_squared = dx * dx + dy * dy;
        return distance_squared <= attack_range_ * attack_range_;
    }
    
    void EnemyTower::ResetCooldown() {
        turns_until_attack_ = cooldown_;
    }
}
