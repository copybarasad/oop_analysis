#include "Cell.h"

Cell::Cell() : type_(CellType::Wall) {}
Cell::Cell(CellType t) : type_(t) {}

Cell::CellType Cell::Type() const { return type_; }
void Cell::SetType(CellType t) { type_ = t; }

bool Cell::IsPassable() const {
    return type_ == CellType::Floor || type_ == CellType::Slow;
}
bool Cell::IsSlow() const { return type_ == CellType::Slow; }
