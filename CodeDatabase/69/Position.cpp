#include "Position.h"

Position::Position(int x_, int y_) : x(x_), y(y_) {}

bool Position::operator==(Position const& o) const { 
    return x == o.x && y == o.y; 
}