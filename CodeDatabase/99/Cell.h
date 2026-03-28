#ifndef CELL_H
#define CELL_H

#include <string>

class Unit;

// Enum representing the state of a cell on the game field
enum class CellState {
	Empty,
	Player,
	Enemy
};

// Cell class - represents a single cell on the game field
class Cell {
private:
	CellState state;
	Unit* occupant;  // Non-owning pointer to the unit occupying this cell

public:
	Cell();

	// State management
	CellState getState() const;
	bool isEmpty() const;

	// Occupant management (non-owning pointers)
	void setOccupant(Unit* unit, CellState state);
	void clear();
	Unit* getOccupant() const;

	// Display
	std::string getDisplaySymbol() const;
};

#endif // CELL_H
