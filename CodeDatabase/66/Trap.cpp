#include "../include/Trap.h"

Trap::Trap(int damage) : damage(damage) {}

int Trap::getDamage() const {
    return damage;
}
