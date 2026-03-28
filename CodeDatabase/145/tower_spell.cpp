#include "tower_spell.h"
#include <cmath>

TowerSpell::TowerSpell(int x, int y, int health, int damage) 
    : GameEntity(health, damage), position(x, y), attackRadius(3), 
      attackCooldown(3), currentCooldown(0) {}

std::pair<int, int> TowerSpell::getPosition() const {
    return position;
}

void TowerSpell::setPosition(int x, int y) {
    position = {x, y};
}

bool TowerSpell::canAttack() const {
    return currentCooldown == 0;
}

void TowerSpell::resetCooldown() {
    currentCooldown = attackCooldown;
}

void TowerSpell::updateCooldown() {
    if (currentCooldown > 0) {
        currentCooldown--;
    }
}

int TowerSpell::getAttackRadius() const {
    return attackRadius;
}

int TowerSpell::getDamage() const {
    return damage;
}

int TowerSpell::getHealth() const {
    return health;
}

void TowerSpell::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

bool TowerSpell::isAlive() const {
    return health > 0;
}