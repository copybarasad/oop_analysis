#include "Cell.h"

Cell::Cell(CellType type) : cellType(type) {
}

CellType Cell::GetType() const {
	return cellType;
}

void Cell::SetType(CellType type) {
	cellType = type;
}

bool Cell::IsPassable() const {
	return isPassableType(cellType);
}

bool Cell::IsSlowing() const {
	return isSlowingType(cellType);
}

char Cell::GetDisplaySymbol() const {
	return getSymbolForType(cellType);
}

bool Cell::isPassableType(CellType type) {
	switch (type) {
		case CellType::NORMAL:
		case CellType::SLOWING:
			return true;
		case CellType::IMPASSABLE:
			return false;
		default:
			return true;
	}
}

bool Cell::isSlowingType(CellType type) {
	return type == CellType::SLOWING;
}

char Cell::getSymbolForType(CellType type) {
	switch (type) {
		case CellType::NORMAL:
			return '.';
		case CellType::IMPASSABLE:
			return '#';
		case CellType::SLOWING:
			return '~';
		default:
			return '.';
	}
}
