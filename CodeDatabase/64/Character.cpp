#include "Character.h"

Character::Character(int health, int damage, int x, int y)
    : hitPoints(health), attackDamage(damage), coordX(x), coordY(y), aliveStatus(true) {
}

int Character::getHitPoints() const {
    return hitPoints;
}

int Character::getAttackDamage() const {
    return attackDamage;
}

int Character::getCoordX() const {
    return coordX;
}

int Character::getCoordY() const {
    return coordY;
}

bool Character::isAlive() const {
    return aliveStatus;
}

void Character::receiveDamage(int damageAmount) {
    hitPoints -= damageAmount;
    if (hitPoints <= 0) {
        hitPoints = 0;
        aliveStatus = false;
    }
}

void Character::changePosition(int newX, int newY) {
    coordX = newX;
    coordY = newY;
}

bool Character::isOnPosition(int x, int y) const {
    return (coordX == x) && (coordY == y);
}