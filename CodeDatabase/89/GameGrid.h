#ifndef GAME_GRID_H
#define GAME_GRID_H

#include "Cell.h"
#include <vector>
#include <random>

class GameGrid {
public:
    GameGrid(int width, int height, int impassableCellsCount);
    
    int getWidth() const;
    int getHeight() const;
    
    bool isValidCoordinate(int x, int y) const;
    Cell& getCell(int x, int y);
    const Cell& getCell(int x, int y) const;
    
    void updateCell(int x, int y, CellType type);
    bool isCellPassable(int x, int y) const;
    bool isCellEmpty(int x, int y) const;
    void printGrid() const;

private:
    int width;
    int height;
    std::vector<std::vector<Cell>> grid;
    
    void initializeGrid();
    void generateImpassableCells(int count);
};

#endif