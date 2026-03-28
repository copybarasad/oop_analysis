#include "position.h"

Position::Position(int xCoord, int yCoord) : x(xCoord), y(yCoord) {}

int Position::getX() const {
    return x;
}

int Position::getY() const {
    return y;
}

void Position::setX(int newX) {
    x = newX;
}

void Position::setY(int newY) {
    y = newY;
}

bool Position::operator==(const Position& other) const {
    return x == other.x && y == other.y;
}

bool Position::operator!=(const Position& other) const {
    return !(*this == other);
}

Position Position::operator+(const Position& other) const {
    return Position(x + other.x, y + other.y);
}
