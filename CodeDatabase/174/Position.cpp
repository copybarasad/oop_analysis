#include "Position.h"

Position::Position() : coordinateX(0), coordinateY(0) {}
Position::Position(int x, int y) : coordinateX(x), coordinateY(y) {}

int Position::getX() const { return coordinateX; }
int Position::getY() const { return coordinateY; }

void Position::setX(int newX) { coordinateX = newX; }
void Position::setY(int newY) { coordinateY = newY; }

bool Position::operator==(const Position& other) const { 
    return coordinateX == other.coordinateX && coordinateY == other.coordinateY; 
}