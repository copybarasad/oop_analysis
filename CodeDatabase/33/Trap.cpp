#include "Trap.h"

Trap::Trap(Coord pos, int damage)
    : pos_(pos), damage_(damage) {}

Coord Trap::Pos() const {
    return pos_;
}

int Trap::Damage() const {
    return damage_;
}
