#include "Cell.h"

Cell::Cell() : type(CellType::Empty) {}

void Cell::setType(CellType newType) {
    type = newType;
}

CellType Cell::getType() const {
    return type;
}

bool Cell::isPassable() const {
    return type != CellType::Impassable && type != CellType::Tower;
}