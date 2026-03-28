#include "Cell.h"

Cell::Cell() : type(CellType::EMPTY), isPassable(true) {}

Cell::Cell(CellType cellType) : type(cellType), isPassable(cellType != CellType::WALL) {}

bool Cell::getIsPassable() const {
    return isPassable;
}

CellType Cell::getType() const {
    return type;
}

void Cell::setType(CellType cellType) {
    type = cellType;
    isPassable = (cellType != CellType::WALL);
}