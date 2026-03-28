#include "Position.h"

Position::Position(int x, int y) : x(x), y(y) {}

int Position::getX() const { return x; }
int Position::getY() const { return y; }

void Position::setX(int x) { this->x = x; }
void Position::setY(int y) { this->y = y; }
void Position::set(int x, int y) { this->x = x; this->y = y; }

bool Position::operator==(const Position& other) const {
    return x == other.x && y == other.y;
}

void Position::move(int dx, int dy) {
    x += dx;
    y += dy;
}