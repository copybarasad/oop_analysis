#include "enemytower.hpp"
#include "gamemodel.hpp"
#include "constants.hpp"
#include <cmath>
#include <iostream>

EnemyTower::EnemyTower(int x, int y, int health, int damage, int radius, int delay)
    : EnemyStructure(x, y, health, damage, delay),
      attackRadius_(radius) {}

void EnemyTower::Tick(Game& game) {
    if (!isAlive()) return;
    counter_++;
    if (counter_ < delay_) return;
    auto& player = game.getPlayer();
    if (!player.isAlive()) return;
    double distance = std::hypot(player.getX() - getX(), player.getY() - getY());
    if (distance <= attackRadius_) {
        ApplyWeakenedDirectDamage(game);
        counter_ = 0; 
    }
}

void EnemyTower::ApplyWeakenedDirectDamage(Game& game) {
    auto& player = game.getPlayer();
    const int weakenedDamage = std::max(1, configs::kDirectDamage / 2);
    player.TakeDamage(weakenedDamage);
}

int EnemyTower::getRadius() const noexcept {
    return attackRadius_;
}

int EnemyTower::getDelay() const noexcept {
    return delay_;
}

int EnemyTower::getCounter() const noexcept {
    return counter_;
}