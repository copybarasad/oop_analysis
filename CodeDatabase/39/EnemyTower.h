#ifndef ENEMY_TOWER_H
#define ENEMY_TOWER_H

#include <cstddef>

class EnemyTower {
public:
    EnemyTower(std::size_t attack_radius,
               std::size_t cooldown_period,
               int damage);

    // Базовые характеристики
    std::size_t GetAttackRadius() const;

    std::size_t GetCooldownPeriod() const;

    std::size_t GetTurnsUntilReady() const;

    int GetDamage() const;

    std::size_t GetCooldown() const; 
    std::size_t GetCurrentTimer() const; 
    void SetCurrentTimer(std::size_t value);

    bool Tick();

    void ResetCooldown();

private:
    std::size_t attack_radius_;
    std::size_t cooldown_period_;
    std::size_t turns_until_ready_;
    int damage_;

    void EnsureInvariants() const;
};

#endif
