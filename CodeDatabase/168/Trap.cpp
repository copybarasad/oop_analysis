#include "Trap.h"

Trap::Trap(int x, int y, int damage)
    : position(x, y), damage(damage) {
}

int Trap::get_Damage() const {
    return damage;
}

const Position& Trap::get_Position() const {
    return position;
}







