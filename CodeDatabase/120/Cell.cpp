#include "Cell.h"

Cell::Cell(CellType initialType) : type(initialType) {}

Cell::CellType Cell::GetType() const { return type; }

void Cell::SetType(CellType newType) { type = newType; }

bool Cell::IsWalkable() const {
  return type == CellType::Empty || type == CellType::Slow;
}
