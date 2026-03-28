#ifndef ENEMY_BASE_H
#define ENEMY_BASE_H

#include <cstddef>

class EnemyBase {
public:
    EnemyBase(std::size_t spawn_period);

    std::size_t GetSpawnPeriod() const;

    std::size_t GetTurnsUntilSpawn() const;

    int GetSpawnCooldown() const { return spawn_period_; }
    int GetCurrentTimer() const { return turns_until_spawn_; }

    void SetCurrentTimer(int t) { turns_until_spawn_ = t; }

    // Возвращает true, если на этом ходу нужно заспавнить врага
    bool Tick();

private:
    std::size_t spawn_period_;
    std::size_t turns_until_spawn_;

    void EnsureInvariants() const;
};

#endif
