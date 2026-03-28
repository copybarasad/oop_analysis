#include "EnemyBase.h"
#include <stdexcept>

EnemyBase::EnemyBase(std::size_t spawn_period)
    : spawn_period_(spawn_period),
      turns_until_spawn_(spawn_period) {
    if (spawn_period_ == 0) {
        throw std::invalid_argument("Spawn period must be greater than zero.");
    }
    EnsureInvariants();
}

std::size_t EnemyBase::GetSpawnPeriod() const {
    return spawn_period_;
}

std::size_t EnemyBase::GetTurnsUntilSpawn() const {
    return turns_until_spawn_;
}

bool EnemyBase::Tick() {
    if (turns_until_spawn_ == 0) {
        turns_until_spawn_ = spawn_period_;
    }
    if (turns_until_spawn_ > 0) {
        --turns_until_spawn_;
    }
    if (turns_until_spawn_ == 0) {
        // На следующем Tick произойдёт перезапуск.
        return true;
    }
    return false;
}

void EnemyBase::EnsureInvariants() const {
    if (spawn_period_ == 0) {
        throw std::logic_error("EnemyBase spawn period must be positive.");
    }
}
