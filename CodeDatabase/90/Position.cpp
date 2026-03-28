#include "Position.h"

Position::Position() : x(0), y(0) {}

Position::Position(int x, int y) : x(x), y(y) {}

int Position::getX() const { return x; }

int Position::getY() const { return y; }

void Position::set(int newX, int newY) {
  x = newX;
  y = newY;
}

void Position::set(const Position &other) {
  x = other.x;
  y = other.y;
}

void Position::moveUp() { y--; }

void Position::moveDown() { y++; }

void Position::moveLeft() { x--; }

void Position::moveRight() { x++; }

int Position::distanceTo(const Position &other) const {
  return (x > other.x ? x - other.x : other.x - x) +
         (y > other.y ? y - other.y : other.y - y);
}

bool Position::operator==(const Position &other) const {
  return x == other.x && y == other.y;
}

bool Position::operator!=(const Position &other) const {
  return !(*this == other);
}
