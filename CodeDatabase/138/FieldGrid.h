#ifndef FIELDGRID_H
#define FIELDGRID_H

#include "Cell.h"
#include "GameTypes.h"
#include <vector>

class FieldGrid {
public:
	FieldGrid(int width, int height);
	
	int GetWidth() const;
	int GetHeight() const;
	
	bool IsValidPosition(int x, int y) const;
	bool IsCellPassable(int x, int y) const;
	bool IsCellSlowing(int x, int y) const;
	
	void SetCellType(int x, int y, CellType type);
	char GetCellSymbol(int x, int y) const;
	
	void Initialize();
	void GenerateImpassableCells();
	void GenerateSlowingCells();
	void EnsureSpawnArea(int spawnX, int spawnY);

private:
	int fieldWidth;
	int fieldHeight;
	std::vector<std::vector<Cell>> gameCells;
	
	static constexpr double IMPASSABLE_CELL_PROBABILITY = 0.15;
	static constexpr double SLOWING_CELL_PROBABILITY = 0.1;
};

#endif

