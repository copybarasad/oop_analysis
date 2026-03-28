#include <random>
#include <iostream>
#include "../include/EnemyBuilding.h"
#include "../include/Enemy.h"

EnemyBuilding::EnemyBuilding(int hp, int spawnInterval, int spawnHp, int spawnDmg)
    : spawnInterval_(spawnInterval), spawnHp_(spawnHp), spawnDmg_(spawnDmg) {
    hp_ = std::max(0, hp);
}

std::unique_ptr<Entity> EnemyBuilding::spawnEnemy() const {
    return std::make_unique<Enemy>(spawnHp_, spawnDmg_);
}

int EnemyBuilding::spawnInterval() const { return spawnInterval_; }

bool EnemyBuilding::timeToSpawn() {
    ++counter_;
    if (counter_ >= spawnInterval_) {
        counter_ = 0;
        return true;
    }
    return false;
}

EntityType EnemyBuilding::type() const { return EntityType::EnemyBuilding; }

void EnemyBuilding::save(std::ostream& os) const {
    Entity::save(os);
    os << spawnInterval_ << " " << counter_ << " " << spawnHp_ << " " << spawnDmg_ << "\n";
}

void EnemyBuilding::load(std::istream& is) {
    Entity::load(is);
    is >> spawnInterval_ >> counter_ >> spawnHp_ >> spawnDmg_;
}
