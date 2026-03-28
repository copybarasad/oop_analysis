#include "enemy_tower.h"
#include "game.h"
#include <cmath>
#include <iostream>

EnemyTower::EnemyTower(int startX, int startY) {
    positionX = startX;
    positionY = startY;
    health = TOWER_HEALTH;
    currentCooldown = 0;
}

int EnemyTower::getPositionX() const {
    return positionX;
}

int EnemyTower::getPositionY() const {
    return positionY;
}

int EnemyTower::getHealth() const {
    return health;
}

int EnemyTower::getCurrentCooldown() const {
    return currentCooldown;
}

int EnemyTower::getAttackRange() const {
    return ATTACK_RANGE;
}

void EnemyTower::takeDamage(int damageAmount) {
    health -= damageAmount;
    if (health <= 0) {
        health = 0;
    }
}

void EnemyTower::update() {
    if (isAlive() && currentCooldown > 0) {
        currentCooldown--;
    }
}

bool EnemyTower::canAttack() const {
    return isAlive() && currentCooldown == 0;
}

void EnemyTower::performAttack(Game& game) {
    if (canAttack() && isPlayerInRange(game.getPlayerPositionX(), game.getPlayerPositionY())) {
        game.damagePlayer(SPELL_DAMAGE);
        currentCooldown = ATTACK_COOLDOWN;
        std::cout << "Enemy tower attacked you for " << SPELL_DAMAGE << " damage!" << std::endl;
    }
}

void EnemyTower::resetCooldown() {
    currentCooldown = 0;
}

bool EnemyTower::isAlive() const {
    return health > 0;
}

bool EnemyTower::isPlayerInRange(int playerX, int playerY) const {
    int distanceX = std::abs(playerX - positionX);
    int distanceY = std::abs(playerY - positionY);
    return distanceX <= ATTACK_RANGE && distanceY <= ATTACK_RANGE;
}

void EnemyTower::setPosition(int x, int y) {
    positionX = x;
    positionY = y;
}

void EnemyTower::setHealth(int h) {
    health = h;
    if (health <= 0) {
        health = 0;
    }
}

void EnemyTower::setCooldown(int c) {
    currentCooldown = c;
}