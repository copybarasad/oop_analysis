#include "EnemyTower.h"
#include <stdexcept>

EnemyTower::EnemyTower(std::size_t attack_radius,
                       std::size_t cooldown_period,
                       int damage)
    : attack_radius_(attack_radius),
      cooldown_period_(cooldown_period),
      turns_until_ready_(0),
      damage_(damage) {
    EnsureInvariants();
}

std::size_t EnemyTower::GetAttackRadius() const {
    return attack_radius_;
}

std::size_t EnemyTower::GetCooldownPeriod() const {
    return cooldown_period_;
}

std::size_t EnemyTower::GetTurnsUntilReady() const {
    return turns_until_ready_;
}

int EnemyTower::GetDamage() const {
    return damage_;
}

std::size_t EnemyTower::GetCooldown() const {
    return cooldown_period_;
}

std::size_t EnemyTower::GetCurrentTimer() const {
    return turns_until_ready_;
}

void EnemyTower::SetCurrentTimer(std::size_t value) {
    turns_until_ready_ = value;
}

bool EnemyTower::Tick() {
    if (turns_until_ready_ > 0) {
        --turns_until_ready_;
    }

    return turns_until_ready_ == 0;
}

void EnemyTower::ResetCooldown() {
    turns_until_ready_ = cooldown_period_;
}

void EnemyTower::EnsureInvariants() const {
    if (attack_radius_ == 0) {
        throw std::logic_error("EnemyTower: attack_radius_ must be > 0");
    }
    if (cooldown_period_ == 0) {
        throw std::logic_error("EnemyTower: cooldown_period_ must be > 0");
    }
    if (turns_until_ready_ > cooldown_period_) {
        throw std::logic_error("EnemyTower: turns_until_ready_ > cooldown_period_");
    }
    if (damage_ <= 0) {
        throw std::logic_error("EnemyTower: damage_ must be > 0");
    }
}
