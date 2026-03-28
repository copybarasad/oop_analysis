#include "Cell.h"

Cell::Cell(int x, int y)
    : position(x, y), type(CellType::EMPTY) {}

Position Cell::getPosition() const {
    return position;
}

CellType Cell::getType() const {
    return type;
}

void Cell::setType(CellType newType) {
    type = newType;
}

bool Cell::isPassable() const {
    return type == CellType::EMPTY || type == CellType::PLAYER || type == CellType::TRAP;
}
