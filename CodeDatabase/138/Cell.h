#ifndef CELL_H
#define CELL_H

#include "GameTypes.h"

class Cell {
public:
	Cell(CellType type = CellType::NORMAL);
	
	CellType GetType() const;
	void SetType(CellType type);
	
	bool IsPassable() const;
	bool IsSlowing() const;
	char GetDisplaySymbol() const;

private:
	CellType cellType;
	
	static bool isPassableType(CellType type);
	static bool isSlowingType(CellType type);
	static char getSymbolForType(CellType type);
};

#endif
