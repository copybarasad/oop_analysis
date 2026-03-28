#include "Cell.h"

Cell::Cell() : type(CellType::NORMAL), occupied(false) {}

Cell::Cell(CellType cellType) : type(cellType), occupied(false) {}

bool Cell::isPassable() const {
    return type != CellType::BLOCKED && !occupied;
}