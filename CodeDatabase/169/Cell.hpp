#pragma once
#include "Enums.hpp"

class Cell {
public:
	Cell(bool walkable = true, CellType type = CellType::Normal);
	Cell(const Cell& other);
	Cell(Cell&& other) noexcept;
	Cell& operator=(const Cell& other);
	Cell& operator=(Cell&& other) noexcept;
	~Cell() = default;

	bool isWalkable() const noexcept;
	void setWalkable(bool v) noexcept;
	OccupantType getOccupant() const noexcept;
	void setOccupant(OccupantType t, int occupantId = -1);
	void clearOccupant() noexcept;
	int getOccupantId() const noexcept;
	CellType getType() const noexcept;
	void setType(CellType t) noexcept;

private:
	bool walkable;
	OccupantType occupant;
	int occupantId;
	CellType type;
};
