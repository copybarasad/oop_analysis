#include "Position.h"


Position::Position(int x, int y) : x(x), y(y) {}

bool Position::operator==(const Position& other) const {
    return x == other.x && y == other.y;
}

void Position::saveTo(SaveFileRAII &out) const{
    out.write(x).write(y);
}

void Position::loadFrom(SaveFileRAII &in){
    in.read(x).read(y);
}