#include "Cell.h"

Cell::Cell()
	: type(CellType::Empty), occupied(false), trap(0) {}

Cell::Cell(CellType type)
	: type(type), occupied(false) {}

Cell::~Cell() = default;



void Cell::SetType(CellType type) {
	this->type = type;
}

CellType Cell::GetType() const {
	return type;
}

bool Cell::IsOccupied() const {
	return occupied;
}


