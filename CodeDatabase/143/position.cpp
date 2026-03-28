#include "position.h"
#include <fstream>

Position::Position(int x, int y) : xCoordinate(x), yCoordinate(y) {}

int Position::getX() const {
    return xCoordinate;
}

int Position::getY() const {
    return yCoordinate;
}

void Position::setX(int x) {
    xCoordinate = x;
}

void Position::setY(int y) {
    yCoordinate = y;
}

void Position::setPosition(int x, int y) {
    xCoordinate = x;
    yCoordinate = y;
}

bool Position::operator==(const Position& other) const {
    return xCoordinate == other.xCoordinate && yCoordinate == other.yCoordinate;
}

bool Position::operator!=(const Position& other) const {
    return !(*this == other);
}

void Position::save(std::ostream& out) const {
    out.write(reinterpret_cast<const char*>(this), sizeof(Position));
}

void Position::load(std::istream& in) {
    in.read(reinterpret_cast<char*>(this), sizeof(Position));
}