#include "Cell.h"

Cell::Cell() : type(CellType::EMPTY) {}

CellType Cell::getType() const {
    return type;
}

void Cell::setType(CellType newType) {
    type = newType;
}