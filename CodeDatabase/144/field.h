#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include <memory>
#include <utility>
#include "cell.h"

class Field {
private:
    std::vector<std::vector<Cell>> grid;
    int width;
    int height;

public:
	struct Snapshot {
		int width = 0;
		int height = 0;
		std::vector<CellType> cells;
	};

    Field(int width, int height);
    
    Field(const Field& other);
    Field(Field&& other) noexcept;
    Field& operator=(const Field& other);
    Field& operator=(Field&& other) noexcept;
    ~Field() = default;
    
    int getWidth() const;
    int getHeight() const;
    Cell getCell(int x, int y) const;
    CellType getCellType(int x, int y) const;
    
    bool canMoveTo(int x, int y) const;
    
    bool canPlaceEntity(int x, int y, int playerX, int playerY) const;
    
    bool isCellPassable(int x, int y) const;
    bool isSlowCell(int x, int y) const;
    bool isValidPosition(int x, int y) const;
    
    void findPathToPlayer(int fromX, int fromY, int playerX, int playerY, int& moveX, int& moveY) const;
    
    bool hasLineOfSight(int fromX, int fromY, int toX, int toY) const;
	
	Snapshot createSnapshot() const;
	void applySnapshot(const Snapshot& snapshot);
};

#endif