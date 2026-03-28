#include "Cell.h"

Cell::Cell() : type_(CellType::kEmpty) {}

CellType Cell::getType() const {
    return type_;
}

void Cell::setType(CellType type) {
    type_ = type;
}