#include "Trap.h"

Trap::Trap(int x, int y, int trapDamage)
    : damage(trapDamage), positionX(x), positionY(y) {}

int Trap::getDamage() const {
    return damage;
}

int Trap::getPositionX() const {
    return positionX;
}

int Trap::getPositionY() const {
    return positionY;
}

bool Trap::isAt(int x, int y) const {
    return positionX == x && positionY == y;
}

