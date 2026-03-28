#include "enemy_tower.h"
#include "player.h"
#include <cmath>
#include "save_manager.h"

EnemyTower::EnemyTower(int x, int y, int damage, int range, int cooldown)
    : position_(x, y), damage_(damage), range_(range),
      cooldown_(cooldown), currentCooldown_(0) {}

Position EnemyTower::getPosition() const { return position_; }

bool EnemyTower::canAttackThisTurn() const {
    return currentCooldown_ == 0;
}

void EnemyTower::attackPlayer(Player& player) const {
    player.takeDamage(damage_);
}

void EnemyTower::tick() {
    if (currentCooldown_ > 0) {
        --currentCooldown_;
    }
}

void EnemyTower::resetCooldown() {
    currentCooldown_ = cooldown_;
}

int EnemyTower::getRange() const {
    return range_;
}

void EnemyTower::save(std::ostream& os) const {
    os << position_.x << " " << position_.y << " " << damage_ << " " << range_ << " " << cooldown_ << " " << currentCooldown_ << "\n";
}

void EnemyTower::load(std::istream& is) {
    is >> position_.x >> position_.y >> damage_ >> range_ >> cooldown_ >> currentCooldown_;
    if (is.fail()) throw SaveDataException("Failed to load enemy tower data");
    std::string leftover;
    std::getline(is, leftover);
}