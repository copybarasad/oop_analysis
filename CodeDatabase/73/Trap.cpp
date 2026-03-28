#include "Trap.h"

Trap::Trap(int damage, int x, int y) : Entity(x, y), damage_(damage) {}

int Trap::getDamage() const {
    return damage_;
}