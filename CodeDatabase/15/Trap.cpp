#include "Trap.h"

Trap::Trap(int x, int y, int damage)
    : damage_(damage) {
    position_.x = x;
    position_.y = y;
}

Point Trap::getPosition() const {
    return position_;
}

int Trap::getDamage() const {
    return damage_;
}

void Trap::save(std::ostream& os) const {
    os << position_.x << " " << position_.y << " " << damage_ << " ";
}

void Trap::load(std::istream& is) {
    is >> position_.x >> position_.y >> damage_;
}