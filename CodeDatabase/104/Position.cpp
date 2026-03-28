#include "Position.h"
#include <cmath>

Position::Position(int x, int y) : x_(x), y_(y) {}

double Position::distanceTo(const Position& other) const {
    int dx = x_ - other.x_;
    int dy = y_ - other.y_;
    return std::sqrt(dx * dx + dy * dy);
}

bool Position::isWithinRange(const Position& other, int range) const {
    int dx = std::abs(x_ - other.x_);
    int dy = std::abs(y_ - other.y_);
    return dx <= range && dy <= range;
}