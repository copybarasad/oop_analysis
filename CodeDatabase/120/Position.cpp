#include "Position.h"

Position::Position(int xValue, int yValue) : x(xValue), y(yValue) {}

bool operator==(const Position& a, const Position& b) {
  return a.x == b.x && a.y == b.y;
}

bool operator!=(const Position& a, const Position& b) { return !(a == b); }
