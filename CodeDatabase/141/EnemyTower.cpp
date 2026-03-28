#include "EnemyTower.h"
#include "Game.h"
#include <stdexcept>

EnemyTower::EnemyTower(int x, int y, int radius, int towerDamage, int towerCooldown, int maxX, int maxY)
    : positionX(x), positionY(y), attackRadius(radius),
      damage(towerDamage), cooldown(towerCooldown), currentCooldown(0) {
    validateCooldown(towerCooldown);

    if (x < 0 || y < 0 || x >= maxX || y >= maxY) {
        throw std::invalid_argument("Tower position is out of bounds");
    }
    if (radius <= 0) {
        throw std::invalid_argument("Attack radius must be positive");
    }
    if (towerDamage <= 0) {
        throw std::invalid_argument("Tower damage must be positive");
    }
}

void EnemyTower::validateCooldown(int towerCooldown) const {
    if (towerCooldown <= 0) {
        throw std::invalid_argument("Tower cooldown must be positive");
    }
}

void EnemyTower::getPosition(int &x, int &y) const {
    x = positionX;
    y = positionY;
}

int EnemyTower::getAttackRadius() const {
    return attackRadius;
}

int EnemyTower::getDamage() const {
    return damage;
}

bool EnemyTower::canAttack() const {
    return currentCooldown <= 0;
}

void EnemyTower::update() {
    if (currentCooldown > 0) {
        --currentCooldown;
    }
}

void EnemyTower::attackPlayer(Game &game) {
    int playerX, playerY;
    game.getPlayerPosition(playerX, playerY);

    game.castTowerSpell(positionX, positionY, playerX, playerY, attackRadius, damage);
    currentCooldown = cooldown;
}

bool EnemyTower::isPlayerInRange(const Game &game) const {
    return game.isPlayerInRange(positionX, positionY, attackRadius);
}

int EnemyTower::getCooldown() const {
    return cooldown;
}

int EnemyTower::getCurrentCooldown() const {
    return currentCooldown;
}
