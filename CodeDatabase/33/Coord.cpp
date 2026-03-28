#include "Coord.h"

Coord::Coord() : x_(0), y_(0) {}
Coord::Coord(int x, int y) : x_(x), y_(y) {}

int Coord::X() const { return x_; }
int Coord::Y() const { return y_; }
void Coord::Set(int x, int y) { x_ = x; y_ = y; }

bool Coord::operator==(const Coord& other) const {
    return x_ == other.x_ && y_ == other.y_;
}
bool Coord::operator!=(const Coord& other) const {
    return !(*this == other);
}
