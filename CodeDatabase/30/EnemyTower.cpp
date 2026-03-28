#include "../include/EnemyTower.h"
#include <iostream>

EnemyTower::EnemyTower(int hp, int damage, int range, int cooldownPeriod)
    : cooldownPeriod_(cooldownPeriod), cooldown_(0), spell_(damage, range, 0) { // старт без зарядов
    hp_ = hp > 0 ? hp : 0;
}

EntityType EnemyTower::type() const { return EntityType::EnemyTower; }
int EnemyTower::damage() const { return spell_.baseDamage(); }
int EnemyTower::range() const { return spell_.range(); }
bool EnemyTower::ready() const { return cooldown_ <= 0; }
void EnemyTower::tick() { if (cooldown_ > 0) --cooldown_; }
void EnemyTower::resetCooldown() { cooldown_ = cooldownPeriod_; }

void EnemyTower::save(std::ostream& os) const {
    Entity::save(os);
    os << cooldownPeriod_ << " " << cooldown_ << "\n";
    spell_.save(os);
}

void EnemyTower::load(std::istream& is) {
    Entity::load(is);
    is >> cooldownPeriod_ >> cooldown_;

    int typeInt;
    is >> typeInt; // считываем тип
    spell_.load(is);
}
