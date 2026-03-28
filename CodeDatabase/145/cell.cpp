#include "cell.h"

Cell::Cell() : type(CellType::EMPTY) {}

Cell::Cell(CellType cellType) : type(cellType) {}

CellType Cell::getType() const {
    return type;
}

void Cell::setType(CellType cellType) {
    type = cellType;
}

bool Cell::isEmpty() const {
    return type == CellType::EMPTY;
}

bool Cell::isPassable() const {
    return type != CellType::IMPASSABLE;
}

bool Cell::isSlowing() const {
    return type == CellType::SLOWING;
}