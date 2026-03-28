#include "EnemyTower.h"
#include "Player.h"
#include <cmath>
#include <iostream>

EnemyTower::EnemyTower(int x, int y, int attack_range, int damage)
    : position_x_(x), position_y_(y), attack_range_(attack_range),
    damage_(damage), can_attack_(true) {
}

void EnemyTower::update() {
    can_attack_ = true;
}

void EnemyTower::tryAttackPlayer(Player& player) {
    if (!can_attack_) return;

    Position player_pos = player.getPosition();
    int dx = std::abs(position_x_ - player_pos.getX());
    int dy = std::abs(position_y_ - player_pos.getY());
    if (dx <= attack_range_ && dy <= attack_range_) {
        player.takeDamage(damage_);
        std::cout << "Tower attacked you for " << damage_ << " damage!\n";
        can_attack_ = false;
    }
}