#include "Cell.h"

Cell::Cell(CellType cellType) : type(cellType) {}

bool Cell::isPassable() const {
    return type == EMPTY;
}

CellType Cell::getType() const {
    return type;
}

void Cell::setType(CellType newType) {
    type = newType;
}