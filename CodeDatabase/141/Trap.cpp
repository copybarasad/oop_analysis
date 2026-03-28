#include "Trap.h"

Trap::Trap(int x, int y, int trapDamage)
    : positionX(x), positionY(y), damage(trapDamage), active(true) {
}

int Trap::getX() const {
    return positionX;
}

int Trap::getY() const {
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
