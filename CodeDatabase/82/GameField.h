#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include "Cell.h"
#include "Position.h"
#include <vector>

class GameField {
private:
    std::vector<std::vector<Cell>> grid;
    int width;
    int height;

public:
    GameField(int fieldWidth, int fieldHeight);
    
    int getWidth() const { 
        return width; 
    }
    int getHeight() const { 
        return height; 
    }
    const Cell& getCell(int x, int y) const;
    
    bool isPositionValid(const Position& position) const;
    bool isCellPassable(const Position& position) const;
    
    void setCellType(const Position& position, CellType cellType);

    void initializeGrid();
    void validateDimensions() const;
};

#endif
