#include "cell.hpp"

Cell::Cell() noexcept : type_(CellType::kFloor) {}

Cell::Cell(CellType type) noexcept : type_(type) {}

bool Cell::IsPassable() const noexcept { return type_ == CellType::kFloor; }

CellType Cell::Type() const noexcept { return type_; }

void Cell::SetType(CellType t) noexcept { type_ = t; }