#pragma once
#include <vector>
#include <memory>
#include "Cell.h"
#include "struct/dataField.h"




enum class Direction { Up, Down, Left, Right, };

class Field {
private:
	int width;
	int height;
	std::vector<std::vector<Cell>> grid;
public:

	std::pair<int,int> FindFreeAdjacentPosition(const std::pair<int,int>& pos) const;
	Field(int w = 10, int h = 10);
	Field(dataField);
	~Field();
	Field(const Field& other);
	Field(Field&& other) noexcept;
	Field& operator=(const Field& other);
	Field& operator=(Field&& other) noexcept;

	int Getwidth() const noexcept;
	int Getheight() const noexcept;
	bool inBounds(const std::pair<int,int>& p) const noexcept;
	void SetCellType(const std::pair<int,int>& p, CellType t);
	bool IsOccupiedAtPos(std::pair<int,int> p) const;
	void ChangeisOccupiedAtpos(std::pair<int,int> p);
	void SetCellOccupied(const std::pair<int,int>& p, bool occ);
	std::pair<int,int> GetRandomFreePosition() const;
	CellType GetCellTypeAt(const std::pair<int,int>& p) const ;

	int GetTrapDamageAtPos(const std::pair<int,int>& p) const;
	void SetTrapAtPos(const std::pair<int,int>& p, int t);
	dataField Save();

	
};
