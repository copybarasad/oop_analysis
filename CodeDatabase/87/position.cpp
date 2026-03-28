#include "position.hpp"

Position::Position(int x, int y) : x_coord(x), y_coord(y){}

int Position::get_x() const{
    return x_coord;
}

int Position::get_y() const{
    return y_coord;
}

bool Position::operator==(const Position& other) const{
    return x_coord == other.x_coord && y_coord == other.y_coord;
}

bool Position::operator!=(const Position& other) const{
    return !(*this == other);
}

Position Position::operator+(const Position& other) const{
    return Position(x_coord + other.x_coord, y_coord + other.y_coord);
}