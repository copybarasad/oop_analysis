#include "EnemyTower.h"
#include <iostream>

EnemyTower::EnemyTower(int health, int damage, int attackRadius)
    : GameObject(health, "Enemy Tower"), damage(damage),
      attackRadius(attackRadius), canAttack(true),
      weakenedDamage(damage / 2), attackCooldown(0) {}

int EnemyTower::getDamage() const {
    return damage;
}

int EnemyTower::getAttackRadius() const {
    return attackRadius;
}

bool EnemyTower::canAttackNow() const {
    return canAttack && attackCooldown == 0;
}

void EnemyTower::resetAttack() {
    canAttack = true;
}

int EnemyTower::getWeakenedDamage() const {
    return weakenedDamage;
}

void EnemyTower::setCooldown(int cooldown) {
    attackCooldown = cooldown;
}

void EnemyTower::updateCooldown() {
    if (attackCooldown > 0) {
        attackCooldown--;
    }
}

bool EnemyTower::isOnCooldown() const {
    return attackCooldown > 0;
}

int EnemyTower::getCooldown() const {
    return attackCooldown;
}

std::string EnemyTower::getType() const {
    return "EnemyTower";
}
