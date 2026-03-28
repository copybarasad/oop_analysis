#include "EnemyTower.h"
#include "SaveManager.h"
#include <stdexcept>
#include <iostream>

EnemyTower::EnemyTower(const Position& pos, int towerHealth, int towerDamage, int range)
    : position(pos), health(towerHealth), maxHealth(towerHealth), 
      damage(towerDamage), attackRange(range), turnsUntilNextAttack(0) {
    
    if (towerHealth <= 0) {
        throw std::invalid_argument("Tower health must be positive");
    }
    if (towerDamage <= 0) {
        throw std::invalid_argument("Tower damage must be positive");
    }
    if (range <= 0) {
        throw std::invalid_argument("Tower attack range must be positive");
    }
}

EnemyTower::EnemyTower(EnemyTower&& other) noexcept
    : position(other.position),
      health(other.health),
      maxHealth(other.maxHealth),
      damage(other.damage),
      attackRange(other.attackRange),
      turnsUntilNextAttack(other.turnsUntilNextAttack) {
}

EnemyTower& EnemyTower::operator=(EnemyTower&& other) noexcept {
    if (this != &other) {
        position = other.position;
        health = other.health;
        maxHealth = other.maxHealth;
        damage = other.damage;
        attackRange = other.attackRange;
        turnsUntilNextAttack = other.turnsUntilNextAttack;
    }
    return *this;
}

Position EnemyTower::getPosition() const {
    return position;
}

int EnemyTower::getHealth() const {
    return health;
}

int EnemyTower::getMaxHealth() const {
    return maxHealth;
}

int EnemyTower::getDamage() const {
    return damage;
}

int EnemyTower::getAttackRange() const {
    return attackRange;
}

bool EnemyTower::isAlive() const {
    return health > 0;
}

bool EnemyTower::canAttackThisTurn() const {
    return turnsUntilNextAttack == 0;
}

int EnemyTower::getCurrentCooldown() const {
    return turnsUntilNextAttack;
}

void EnemyTower::takeDamageInternal(int dmg) {
    if (dmg < 0) {
        throw std::invalid_argument("Damage cannot be negative");
    }
    
    health -= dmg;
    if (health < 0) {
        health = 0;
    }
}

void EnemyTower::takeDamage(int dmg) {
    takeDamageInternal(dmg);
}

void EnemyTower::resetAttackCooldownInternal() {
    turnsUntilNextAttack = 2;
}

void EnemyTower::decreaseCooldownInternal() {
    if (turnsUntilNextAttack > 0) {
        turnsUntilNextAttack--;
    }
}

void EnemyTower::decreaseCooldown() {
    decreaseCooldownInternal();
}

void EnemyTower::setCooldownInternal(int cooldown) {
    if (cooldown < 0) {
        throw std::invalid_argument("Cooldown cannot be negative");
    }
    
    if (cooldown > 10) {
        std::cerr << "Warning: Suspiciously high cooldown (" << cooldown 
                  << ") for tower, capping at 10\n";
        turnsUntilNextAttack = 10;
    } else {
        turnsUntilNextAttack = cooldown;
    }
}