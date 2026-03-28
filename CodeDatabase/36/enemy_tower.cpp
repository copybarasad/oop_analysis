#include "enemy_tower.h"
#include <algorithm>

EnemyTower::EnemyTower(int row, int col, int health, int attack_radius, int damage, int cast_interval)
    : Character(health, damage), row_(row), col_(col), attack_radius_(attack_radius), 
      cast_interval_(cast_interval), counter_(0) {}

std::pair<int,int> EnemyTower::Position() const { return {row_, col_}; }
int EnemyTower::DirectDamageSpell() const { return Character::Damage(); }

bool EnemyTower::ShouldCast(const std::pair<int,int>& player_pos) {
    int dist = std::max(std::abs(player_pos.first - row_), std::abs(player_pos.second - col_));
    if (dist <= attack_radius_) {
        if (++counter_ >= cast_interval_) {
            counter_ = 0;
            return true;
        }
    } else {
        counter_ = 0;
    }
    return false;
}


