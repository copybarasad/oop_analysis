#include "enemy_base.h"
#include <algorithm>

EnemyBase::EnemyBase(int row, int col, int spawn_interval, int health)
    : Character(health, 0), row_(row), col_(col), spawn_interval_(std::max(1, spawn_interval)), counter_(0) {}

std::pair<int,int> EnemyBase::Position() const { return {row_, col_}; }

bool EnemyBase::ShouldSpawn() {
    if (++counter_ >= spawn_interval_) {
        counter_ = 0;
        return true;
    }
    return false;
}

void EnemyBase::SetCounter(int counter) {
    counter_ = counter;
}
