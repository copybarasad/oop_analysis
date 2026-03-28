#include "EnemyTower.h"
#include <cmath>
#include <iostream>

EnemyTower::EnemyTower(int x, int y) 
    : pos(x, y), health(MAX_HEALTH), attackCooldown(0) {}

void EnemyTower::takeDamage(int damage) {
    if (damage > 0) {
        health -= damage;
    }
}

bool EnemyTower::canAttack(const Position& playerPos) const {
    if (attackCooldown > 0 || !isAlive()) {
        return false;
    }
    
    int dx = std::abs(pos.getX() - playerPos.getX());
    int dy = std::abs(pos.getY() - playerPos.getY());
    int distance = dx + dy;
    
    return distance <= ATTACK_RANGE;
}

void EnemyTower::attack(Player& player) {
    if (canAttack(player.getPos())) {
        player.takeDamage(ATTACK_DAMAGE);
        std::cout << "Enemy tower hit you with fireball for " << ATTACK_DAMAGE << " damage!" << std::endl;
        attackCooldown = COOLDOWN_TURNS;
    }
}

void EnemyTower::updateCooldown() {
    if (attackCooldown > 0) {
        attackCooldown--;
    }
}

const Position& EnemyTower::getPos() const {
    return pos;
}

int EnemyTower::getHealth() const {
    return health;
}

bool EnemyTower::isAlive() const {
    return health > 0;
}