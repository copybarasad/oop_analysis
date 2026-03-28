#ifndef CELL_H
#define CELL_H

enum class CellType {
	EMPTY,
	WALL,
	SLOW
};

class Cell {
private:
	CellType type;

public:
	Cell(CellType cellType = CellType::EMPTY);
	
	CellType getType() const;
	void setType(CellType cellType);
	
	bool isPassable() const;
};

#endif