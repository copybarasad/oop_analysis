#include "EnemyTower.h"
#include <cmath>

EnemyTower::EnemyTower(int initialHealth, int towerDamage, int range) 
    : Enemy(initialHealth, towerDamage), attackRange(range), attackCooldown(0) {}

bool EnemyTower::isBuilding() const {
    return true;
}

bool EnemyTower::canAttack() const {
    return attackCooldown <= 0;
}


void EnemyTower::resetCooldown() {
    attackCooldown = 0;
}

int EnemyTower::getAttackRange() const {
    return attackRange;
}

void EnemyTower::decreaseCooldown() {
    if (attackCooldown > 0) {
        attackCooldown--;
    }
}

int EnemyTower::getCooldown() const {
    return attackCooldown;
}

void EnemyTower::setCooldown(int cooldown) {
    attackCooldown = cooldown;
}