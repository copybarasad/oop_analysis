#ifndef FIELD_H
#define FIELD_H

#include "Cell.h"
#include <vector>
#include <unordered_map>
#include <string>

class Unit;

// Position structure - represents coordinates on the field
struct Position {
	int x;  // Column (0 to width-1)
	int y;  // Row (0 to height-1)

	bool operator==(const Position& other) const;
};

// Hash function for Position to use in unordered_map
namespace std {
	template<>
	struct hash<Position> {
		size_t operator()(const Position& pos) const {
			return hash<int>()(pos.x) ^ (hash<int>()(pos.y) << 1);
		}
	};
}

// Direction enum - represents movement directions
enum class Direction {
	Up,
	Down,
	Left,
	Right
};

// Field class - manages the game field grid and unit positions
class Field {
private:
	int width;
	int height;
	std::vector<std::vector<Cell>> grid;
	std::unordered_map<Unit*, Position> unitPositions;

public:
	// Constructors
	Field(int width, int height);
	Field(const Field& other);  // Deep copy
	Field(Field&& other) noexcept;  // Move

	// Assignment operators
	Field& operator=(const Field& other);  // Deep copy
	Field& operator=(Field&& other) noexcept;  // Move

	~Field();

	// Dimensions
	int getWidth() const;
	int getHeight() const;

	// Position queries (no raw pointer exposure)
	bool isValidPosition(const Position& pos) const;
	bool isOccupied(const Position& pos) const;
	CellState getCellState(const Position& pos) const;
	Position getUnitPosition(const Unit* unit) const;

	// Unit placement
	bool placeUnit(Unit* unit, const Position& pos);
	void removeUnit(Unit* unit);

	// Movement
	bool moveUnit(Unit* unit, Direction dir);
	Position getAdjacentPosition(const Position& pos, Direction dir) const;
	std::vector<Direction> getValidDirections(const Position& pos) const;

	// Combat
	Unit* getUnitAt(const Position& pos) const;

	// Display
	std::string render() const;
};

#endif // FIELD_H
