#include "Position.h"
#include <fstream>


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

void Position::move(int deltaX, int deltaY) {
    x += deltaX;
    y += deltaY;
}

bool Position::operator==(const Position& other) const {
    return x == other.x && y == other.y;
}

bool Position::isValid(int maxWidth, int maxHeight) const {
    return x >= 0 && x < maxWidth && y >= 0 && y < maxHeight;
}

void Position::save(std::ofstream& file) const {
    file.write(reinterpret_cast<const char*>(&x), sizeof(x));
    file.write(reinterpret_cast<const char*>(&y), sizeof(y));
}

void Position::load(std::ifstream& file) {
    file.read(reinterpret_cast<char*>(&x), sizeof(x));
    file.read(reinterpret_cast<char*>(&y), sizeof(y));
}