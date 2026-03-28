#ifndef GRID_H
#define GRID_H

#include "Cell.h"
#include "Position.h"
#include <vector>

class Grid {
private:
    std::vector<std::vector<Cell>> grid;
    int width;
    int height;

    void initializeWalls();

public:
    Grid(int width, int height);
    
    Grid(const Grid& other);
    Grid(Grid&& other) noexcept;
    Grid& operator=(const Grid& other);
    Grid& operator=(Grid&& other) noexcept;
    ~Grid() = default;
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
    CellType getCellType(const Position& pos) const;
    bool isValidPosition(const Position& pos) const;
    void setCellType(const Position& pos, CellType cellType);
    bool isPositionPassable(const Position& pos) const;
    bool isPositionEmpty(const Position& pos) const;
    
    Position findRandomEmptyPosition() const;
};

#endif