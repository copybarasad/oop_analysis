#include "Cell.h"

Cell::Cell(CellType type) : type(type) {}

CellType Cell::getType() const {
    return type;
}

bool Cell::isPassable() const {
    return type != CellType::IMPASSABLE;
}

bool Cell::isSlowing() const {
    return type == CellType::SLOWING;
}

void Cell::setType(CellType type) {
    this->type = type;
}
