#include "enemy_tower.h"
#include "player.h"
#include "game_field.h"
#include <iostream>
#include <cmath>

EnemyTower::EnemyTower(Position pos, int damage, int range, int cooldown)
    : position_(pos), health_(50), damage_(damage), range_(range),
      cooldown_(cooldown), current_cooldown_(0) {}

bool EnemyTower::IsAlive() const {
    return health_ > 0;  
}

void EnemyTower::Update(Player& player, GameField& field) {
    if (!IsAlive()) return;

    if (current_cooldown_ > 0) {
        --current_cooldown_;
        return;
    }

    if (!CanSeePlayer(player)) return;

    Shoot(player);
    current_cooldown_ = cooldown_;  
}

bool EnemyTower::CanSeePlayer(const Player& player) const {
    int dist = std::abs(player.GetPosition().x - position_.x) +
               std::abs(player.GetPosition().y - position_.y);
    return dist <= range_;
}

void EnemyTower::Shoot(Player& player) {
    player.TakeDamage(damage_);
    std::cout << "Башня на (" << position_.x << "," << position_.y
              << ") стреляет! Нанесено " << damage_ << " урона!\n";
}

void EnemyTower::TakeDamage(int damage) {
    health_ -= damage;
    if (health_ < 0) health_ = 0;
}
