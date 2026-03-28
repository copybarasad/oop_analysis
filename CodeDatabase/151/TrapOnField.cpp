#include "TrapOnField.h"
#include <cmath>

TrapOnField::TrapOnField(int x, int y, int damage)
    : position(x, y), damage(damage), active(true) {
}

bool TrapOnField::checkTrigger(const Position& target) const {
    return active && position.x == target.x && position.y == target.y;
}

int TrapOnField::getDamage() const {
    return damage;
}

Position TrapOnField::getPosition() const {
    return position;
}

bool TrapOnField::isActive() const {
    return active;
}

void TrapOnField::deactivate() {
    active = false;
}