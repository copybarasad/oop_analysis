#include "cell.h"

Cell::Cell() : position(0, 0), type(CellType::EMPTY) {}

Cell::Cell(int x, int y, CellType type) 
    : position(x, y), type(type) {}

CellType Cell::getType() const {
    return type;
}

Position Cell::getPosition() const {
    return position;
}

bool Cell::isPassable() const {
    return type == CellType::EMPTY;
}

bool Cell::isEmpty() const {
    return type == CellType::EMPTY;
}