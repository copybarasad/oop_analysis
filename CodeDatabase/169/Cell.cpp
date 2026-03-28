#include "Cell.hpp"

Cell::Cell(bool walkable, CellType type)
	: walkable(walkable),
	  occupant(OccupantType::None),
	  occupantId(-1),
	  type(type) {}

Cell::Cell(const Cell& other)
	: walkable(other.walkable),
	  occupant(other.occupant), 
	  occupantId(other.occupantId), 
	  type(other.type) {}

Cell::Cell(Cell&& other) noexcept
	: walkable(other.walkable), occupant(other.occupant), occupantId(other.occupantId), type(other.type) {
	other.walkable = true;
	other.occupant = OccupantType::None;
	other.occupantId = -1;
	other.type = CellType::Normal;
}

Cell& Cell::operator=(const Cell& other) {
	if (this != &other) {
		walkable = other.walkable;
		occupant = other.occupant;
		occupantId = other.occupantId;
		type = other.type;
	}
	return *this;
}

Cell& Cell::operator=(Cell&& other) noexcept {
	if (this != &other) {
		walkable = other.walkable;
		occupant = other.occupant;
		occupantId = other.occupantId;
		type = other.type;

		other.walkable = true;
		other.occupant = OccupantType::None;
		other.occupantId = -1;
		other.type = CellType::Normal;
	}
	return *this;
}

bool Cell::isWalkable() const noexcept { 
	return walkable; 
}

void Cell::setWalkable(bool v) noexcept { 
	walkable = v; 
}

OccupantType Cell::getOccupant() const noexcept { 
	return occupant; 
}

void Cell::setOccupant(OccupantType t, int occupantId) {
	occupant = t;
	this->occupantId = occupantId;
}

void Cell::clearOccupant() noexcept {
	occupant = OccupantType::None;
	occupantId = -1;
}

int Cell::getOccupantId() const noexcept { 
	return occupantId; 
}

CellType Cell::getType() const noexcept { 
	return type; 
}

void Cell::setType(CellType t) noexcept {
	type = t;
}
