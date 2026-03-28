#include "../h/Field.h"
#include "../h/Player.h"
//#include "../h/Enemy.h"

using namespace std;

bool Position::operator==(const Position& other) const {
	return x == other.x && y == other.y;
}

// Constructor with dimension validation
// Ensures field size is between 10x10 and 25x25
Field::Field(int width, int height) {
	// Validate and clamp dimensions to [10, 25] range
	this->width = width;
	this->height = height;
	
	if (this->width < 10) {
		this->width = 10;
	}
	if (this->width > 25) {
		this->width = 25;
	}
	
	if (this->height < 10) {
		this->height = 10;
	}
	if (this->height > 25) {
		this->height = 25;
	}
	
	// Initialize grid with proper dimensions
	grid.resize(this->height);
	for (int i = 0; i < this->height; i++) {
		grid[i].resize(this->width);
	}
}

// Copy constructor - performs deep copy of grid
// Note: unitPositions map contains non-owning pointers, so we copy the map structure
// but the pointers still point to the original units (ownership is external)
Field::Field(const Field& other) : width(other.width), height(other.height) {
	// Deep copy the grid
	grid = other.grid;
	
	// Copy the unitPositions map (shallow copy of pointers, which is correct
	// since Field doesn't own the units)
	unitPositions = other.unitPositions;
}


// Copy assignment operator using copy-and-swap idiom
// Provides strong exception safety
Field& Field::operator=(const Field& other) {
	if (this != &other) {
		// Create temporary copy
		Field temp(other);

		// Swap with temporary
		std::swap(width, temp.width);
		std::swap(height, temp.height);
		std::swap(grid, temp.grid);
		std::swap(unitPositions, temp.unitPositions);
	}
	return *this;
}

// Move assignment operator
Field& Field::operator=(Field&& other) noexcept {
	if (this != &other) {
		width = other.width;
		height = other.height;
		grid = std::move(other.grid);
		unitPositions = std::move(other.unitPositions);
		
		// Leave other in valid state
		other.width = 0;
		other.height = 0;
	}
	return *this;
}

Field::~Field() {
	// No manual cleanup needed - vector and unordered_map handle their own memory
}

int Field::getWidth() const {
	return width;
}

int Field::getHeight() const {
	return height;
}

// Checks if a position is within field boundaries
bool Field::isValidPosition(const Position& pos) const {
	return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
}

// Checks if a position is occupied by a unit
bool Field::isOccupied(const Position& pos) const {
	if (!isValidPosition(pos)) {
		return false;
	}
	return !grid[pos.y][pos.x].isEmpty();
}

// Returns the state of a cell at given position
CellState Field::getCellState(const Position& pos) const {
	if (!isValidPosition(pos)) {
		return CellState::Empty;
	}
	return grid[pos.y][pos.x].getState();
}

// Returns the position of a unit on the field
// Position is stored only in Field, not duplicated in Unit (requirement 6.3)
Position Field::getUnitPosition(const Unit* unit) const {
	auto it = unitPositions.find(const_cast<Unit*>(unit));
	if (it != unitPositions.end()) {
		return it->second;
	}
	// Return invalid position if unit not found
	return {-1, -1};
}

// Places a unit on the field at specified position
// Maintains invariant: unitPositions always matches grid state
bool Field::placeUnit(Unit* unit, const Position& pos) {
	if (!isValidPosition(pos)) {
		return false;
	}
	
	if (isOccupied(pos)) {
		return false;
	}
	
	// Determine cell state based on unit type
	CellState state = (dynamic_cast<Player*>(unit) != nullptr) ? CellState::Player : CellState::Enemy;
	
	// Update grid cell
	grid[pos.y][pos.x].setOccupant(unit, state);
	
	// Update position map
	unitPositions[unit] = pos;
	
	return true;
}

// Removes a unit from the field
// Maintains invariant: unitPositions always matches grid state
void Field::removeUnit(Unit* unit) {
	auto it = unitPositions.find(unit);
	if (it == unitPositions.end()) {
		return;
	}
	
	Position pos = it->second;
	
	// Clear the cell
	grid[pos.y][pos.x].clear();
	
	// Remove from position map
	unitPositions.erase(it);
}

// Moves a unit in specified direction
// Returns false if movement is invalid (boundary or occupied)
bool Field::moveUnit(Unit* unit, Direction dir) {
	auto it = unitPositions.find(unit);
	if (it == unitPositions.end()) {
		return false;
	}
	
	Position currentPos = it->second;
	Position newPos = getAdjacentPosition(currentPos, dir);
	
	// Check if new position is valid
	if (!isValidPosition(newPos)) {
		return false;
	}
	
	// Check if new position is occupied
	if (isOccupied(newPos)) {
		return false;
	}
	
	// Get cell state for this unit
	CellState state = grid[currentPos.y][currentPos.x].getState();
	
	// Clear old position
	grid[currentPos.y][currentPos.x].clear();
	
	// Set new position
	grid[newPos.y][newPos.x].setOccupant(unit, state);
	
	// Update position map
	unitPositions[unit] = newPos;
	
	return true;
}

// Calculates adjacent position based on direction
Position Field::getAdjacentPosition(const Position& pos, Direction dir) const {
	Position newPos = pos;
	
	switch (dir) {
		case Direction::Up:
			newPos.y -= 1;
			break;
		case Direction::Down:
			newPos.y += 1;
			break;
		case Direction::Left:
			newPos.x -= 1;
			break;
		case Direction::Right:
			newPos.x += 1;
			break;
	}
	
	return newPos;
}

// Returns list of valid movement directions from a position
vector<Direction> Field::getValidDirections(const Position& pos) const {
	vector<Direction> validDirs;
	
	vector<Direction> allDirs = {Direction::Up, Direction::Down, Direction::Left, Direction::Right};
	
	for (Direction dir : allDirs) {
		Position adjacent = getAdjacentPosition(pos, dir);
		if (isValidPosition(adjacent) && !isOccupied(adjacent)) {
			validDirs.push_back(dir);
		}
	}
	
	return validDirs;
}

// Returns the unit at specified position (for combat resolution)
Unit* Field::getUnitAt(const Position& pos) const {
	if (!isValidPosition(pos)) {
		return nullptr;
	}
	
	return grid[pos.y][pos.x].getOccupant();
}

// Renders the field as a string for CLI display with coordinates
string Field::render() const {
	string result;
	
	// Top X-axis coordinates (with spacing for wider cells)
	result += "    ";  // Spacing for Y-axis
	for (int x = 0; x < width; x++) {
		result += " ";
		result += to_string(x % 10);
	}
	result += "\n";
	
	// Top border
	result += "   +";
	for (int x = 0; x < width; x++) {
		result += "--";
	}
	result += "+\n";
	
	// Grid rows with Y-axis coordinates
	for (int y = 0; y < height; y++) {
		// Y coordinate (right-aligned for single digit)
		if (y < 10) {
			result += " ";
		}
		result += to_string(y);
		result += " |";
		
		// Grid cells with spacing
		for (int x = 0; x < width; x++) {
			result += " ";
			result += grid[y][x].getDisplaySymbol();
		}
		result += " |\n";
	}
	
	// Bottom border
	result += "   +";
	for (int x = 0; x < width; x++) {
		result += "--";
	}
	result += "+\n";
	
	return result;
}
