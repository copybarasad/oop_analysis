#include "Position.h"

Position::Position(int xCoord, int yCoord) : x(xCoord), y(yCoord) {}

bool Position::operator==(const Position &other) const { return x==other.x && y==other.y; }
bool Position::operator!=(const Position &other) const { return !(*this==other); }