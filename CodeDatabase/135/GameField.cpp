#include "GameField.h"
#include "GameConstants.h"
#include <stdexcept>

void GameField::initializeCells(RandomGenerator& rng) {
	for (int y = 0; y < height_field; ++y) {
		for (int x = 0; x < width_field; ++x) {
			CellType cell_type = CellType::NORMAL;
			
			int rand_val = rng.getInt(0, GameConstants::RANDOM_RANGE_MAX);
			if (rand_val < GameConstants::IMPASSABLE_CELL_PROBABILITY) {
				cell_type = CellType::IMPASSABLE;
			}
			else if (rand_val < GameConstants::IMPASSABLE_CELL_PROBABILITY + GameConstants::SLOWING_CELL_PROBABILITY) {
				cell_type = CellType::SLOWING;
			}
			
			cells.emplace_back(cell_type);
		}
	}
	
	int player_start_x = 1;
	int player_start_y = 1;
	int player_index = coordToIndex(player_start_x, player_start_y);
	cells[player_index] = FieldCell(CellType::NORMAL);
}

GameField::GameField(int width, int height)
	: width_field(width), height_field(height) {
	
	if (width < 10 || width > 25 || height < 10 || height > 25) {
		throw std::invalid_argument("Field size must be between 10x10 and 25x25");
	}
	
	RandomGenerator rng;
	initializeCells(rng);
}

GameField::GameField(int width, int height, RandomGenerator& rng)
	: width_field(width), height_field(height) {
	
	if (width < 10 || width > 25 || height < 10 || height > 25) {
		throw std::invalid_argument("Field size must be between 10x10 and 25x25");
	}
	
	initializeCells(rng);
}

GameField GameField::createEmpty(int width, int height) {
	if (width < 10 || width > 25 || height < 10 || height > 25) {
		throw std::invalid_argument("Field size must be between 10x10 and 25x25");
	}
	
	GameField field(width, height);
	field.cells.clear();
	field.cells.reserve(width * height);
	
	for (int i = 0; i < width * height; ++i) {
		field.cells.emplace_back(CellType::NORMAL);
	}
	
	return field;
}

int GameField::coordToIndex(int x, int y) const {
	return y * width_field + x;
}

void GameField::indexToCoord(int index, int& x, int& y) const {
	x = index % width_field;
	y = index / width_field;
}

bool GameField::isValidPosition(int x, int y) const {
	return x >= 0 && x < width_field && y >= 0 && y < height_field;
}

FieldCell& GameField::getCell(int index) {
	if (index < 0 || index >= static_cast<int>(cells.size())) {
		throw std::out_of_range("Cell index out of range");
	}
	return cells[index];
}

const FieldCell& GameField::getCell(int index) const {
	if (index < 0 || index >= static_cast<int>(cells.size())) {
		throw std::out_of_range("Cell index out of range");
	}
	return cells[index];
}

void GameField::setCellType(int index, CellType type) {
	if (index < 0 || index >= static_cast<int>(cells.size())) {
		throw std::out_of_range("Cell index out of range");
	}
	cells[index] = FieldCell(type);
}

int GameField::getWidth() const {
	return width_field;
}

int GameField::getHeight() const {
	return height_field;
}
