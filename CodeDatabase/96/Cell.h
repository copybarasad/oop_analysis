#pragma once
#include "Entity.h"
#include <string>
#include "CellType.h"


class Cell {
private:
	CellType type;
	bool occupied;
	int trap;
public:
	Cell();
	Cell(CellType type);
	~Cell();

	void SetType(CellType type);
	CellType GetType() const;
	bool IsOccupied() const;
	void SetOccupied(bool occ) { occupied = occ; }
	int GetTrapDamage() const { return trap; }
	void SetTrap(int t) { trap = t; }
};
