#include "Trap.h"

Trap::Trap(const Position& pos, int trapDamage)
    : position(pos), damage(trapDamage), trapActive(true) {}

Position Trap::getPosition() const {
    return position;
}

int Trap::getDamage() const {
    return damage;
}

bool Trap::isActive() const {
    return trapActive;
}

void Trap::trigger() {
    trapActive = false;
}
