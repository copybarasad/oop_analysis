#include "trap.h"

Trap::Trap(int x, int y, int trapDamage) 
    : positionX(x), positionY(y), damage(trapDamage), active(true) {}

int Trap::getPositionX() const {
    return positionX;
}

int Trap::getPositionY() const {
    return positionY;
}

int Trap::getDamage() const {
    return damage;
}

bool Trap::isActive() const {
    return active;
}

void Trap::trigger() {
    active = false;
}

