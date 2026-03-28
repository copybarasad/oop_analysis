#include "cell.h"

Cell::Cell(CellType cellType) : type(cellType) {}

CellType Cell::getType() const {
	return type;
}

void Cell::setType(CellType cellType) {
	type = cellType;
}

bool Cell::isPassable() const {
	return type != CellType::WALL;
}