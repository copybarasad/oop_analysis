#include "trap.h"

Trap::Trap(int x, int y, int damage)
    : position_x_(x), position_y_(y), damage_(damage) {}

int Trap::get_x() const {
    return position_x_;
}

int Trap::get_y() const {
    return position_y_;
}

int Trap::get_damage() const {
    return damage_;
}

bool Trap::is_at_position(int x, int y) const {
    return position_x_ == x && position_y_ == y;
}