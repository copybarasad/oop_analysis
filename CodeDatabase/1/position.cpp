#include "position.h"
#include <stdexcept>
#include <cmath>

Position::Position(int x, int y) : x_(x), y_(y) {
    if (x < 0 || y < 0) {
        throw std::invalid_argument("Coordinates cannot be negative");
    }
}

int Position::getX() const { return x_; }
int Position::getY() const { return y_; }

Position Position::translated(int dx, int dy) const {
    return Position(x_ + dx, y_ + dy);
}

int Position::deltaX(const Position& other) const {
    return other.x_ - x_;
}

int Position::deltaY(const Position& other) const {
    return other.y_ - y_;
}

int Position::manhattanDistance(const Position& other) const {
    return std::abs(deltaX(other)) + std::abs(deltaY(other));
}

bool Position::operator==(const Position& other) const {
    return x_ == other.x_ && y_ == other.y_;
}

bool Position::operator!=(const Position& other) const {
    return !(*this == other);
}