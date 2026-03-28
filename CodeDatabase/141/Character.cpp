#include "Character.h"

Character::Character(int startX, int startY, int initialHealth, int maxX, int maxY)
    : health(initialHealth), positionX(startX), positionY(startY) {
    validateHealth(initialHealth);
    validatePosition(startX, startY, maxX, maxY);
}

int Character::getHealth() const {
    return health;
}

void Character::getPosition(int &x, int &y) const {
    x = positionX;
    y = positionY;
}

void Character::takeDamage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

void Character::setPosition(int x, int y) {
    positionX = x;
    positionY = y;
}

bool Character::isAlive() const {
    return health > 0;
}
