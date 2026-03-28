#include "../h/Cell.h"
#include "../h/Unit.h"

using namespace std;

Cell::Cell() : state(CellState::Empty), occupant(nullptr) {
}

CellState Cell::getState() const {
	return state;
}

bool Cell::isEmpty() const {
	return state == CellState::Empty;
}

// Sets the occupant of this cell
// Maintains invariant: if state is Empty, occupant must be nullptr
// if state is not Empty, occupant must not be nullptr
void Cell::setOccupant(Unit* unit, CellState newState) {
	this->occupant = unit;
	this->state = newState;
}

// Clears the cell, removing any occupant
// Maintains invariant: Empty state means nullptr occupant
void Cell::clear() {
	occupant = nullptr;
	state = CellState::Empty;
}

Unit* Cell::getOccupant() const {
	return occupant;
}

// Returns the display symbol for this cell
// Uses the cell state to determine symbol
string Cell::getDisplaySymbol() const {
	switch (state) {
		case CellState::Empty:
			return ".";
		case CellState::Player:
			return "P";
		case CellState::Enemy:
			return "E";
		default:
			return ".";
	}
}
