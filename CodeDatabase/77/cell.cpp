#include "cell.h"

Cell::Cell(CellType type) : type(type) {}

CellType Cell::getType() const {
    return type;
}

char Cell::getSymbol() const {
    return static_cast<char>(type);
}