#include "Cell.h"

Cell::Cell() : type_(CellType::kEmpty) {}

void Cell::setType(CellType type) {
    type_ = type;
}

CellType Cell::getType() const {
    return type_;
}