#include "FieldGrid.h"
#include <random>
#include <stdexcept>

FieldGrid::FieldGrid(int width, int height) 
	: fieldWidth(width), fieldHeight(height) {
	if (width <= 0 || height <= 0) {
		throw std::invalid_argument("Field dimensions must be positive");
	}
	gameCells.resize(fieldHeight);
	for (int y = 0; y < fieldHeight; ++y) {
		gameCells[y].resize(fieldWidth);
		for (int x = 0; x < fieldWidth; ++x) {
			gameCells[y][x] = Cell(CellType::NORMAL);
		}
	}
}

int FieldGrid::GetWidth() const {
	return fieldWidth;
}

int FieldGrid::GetHeight() const {
	return fieldHeight;
}

bool FieldGrid::IsValidPosition(int x, int y) const {
	return x >= 0 && x < fieldWidth && y >= 0 && y < fieldHeight;
}

bool FieldGrid::IsCellPassable(int x, int y) const {
	if (!IsValidPosition(x, y)) {
		return false;
	}
	return gameCells[y][x].IsPassable();
}

bool FieldGrid::IsCellSlowing(int x, int y) const {
	if (!IsValidPosition(x, y)) {
		return false;
	}
	return gameCells[y][x].IsSlowing();
}

void FieldGrid::SetCellType(int x, int y, CellType type) {
	if (!IsValidPosition(x, y)) {
		throw std::invalid_argument("Invalid cell position");
	}
	gameCells[y][x].SetType(type);
}

char FieldGrid::GetCellSymbol(int x, int y) const {
	if (!IsValidPosition(x, y)) {
		return '?';
	}
	return gameCells[y][x].GetDisplaySymbol();
}

void FieldGrid::Initialize() {
	GenerateImpassableCells();
	GenerateSlowingCells();
}

void FieldGrid::GenerateImpassableCells() {
	std::random_device randomDevice;
	std::mt19937 generator(randomDevice());
	std::uniform_real_distribution<double> distribution(0.0, 1.0);
	
	for (int y = 0; y < fieldHeight; ++y) {
		for (int x = 0; x < fieldWidth; ++x) {
			if (distribution(generator) < IMPASSABLE_CELL_PROBABILITY) {
				gameCells[y][x].SetType(CellType::IMPASSABLE);
			}
		}
	}
}

void FieldGrid::GenerateSlowingCells() {
	std::random_device randomDevice;
	std::mt19937 generator(randomDevice());
	std::uniform_real_distribution<double> distribution(0.0, 1.0);
	
	for (int y = 0; y < fieldHeight; ++y) {
		for (int x = 0; x < fieldWidth; ++x) {
			if (gameCells[y][x].GetType() == CellType::NORMAL) {
				if (distribution(generator) < SLOWING_CELL_PROBABILITY) {
					gameCells[y][x].SetType(CellType::SLOWING);
				}
			}
		}
	}
}

void FieldGrid::EnsureSpawnArea(int spawnX, int spawnY) {
	if (!IsValidPosition(spawnX, spawnY)) {
		return;
	}
	
	gameCells[spawnY][spawnX].SetType(CellType::NORMAL);
	
	for (int dy = -1; dy <= 1; ++dy) {
		for (int dx = -1; dx <= 1; ++dx) {
			if (dx == 0 && dy == 0) continue;
			int x = spawnX + dx;
			int y = spawnY + dy;
			if (IsValidPosition(x, y)) {
				gameCells[y][x].SetType(CellType::NORMAL);
			}
		}
	}
}

