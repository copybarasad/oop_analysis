#include "position.h"
#include <cmath>

Position::Position(int pos_x, int pos_y) : x(pos_x), y(pos_y) {}

int Position::getX() const { return x; }
int Position::getY() const { return y; }

bool Position::equals(const Position& other) const {
    return x == other.x && y == other.y;
}

