#ifndef CELL_H
#define CELL_H

#include "game_entity.h"

enum class CellType {
    EMPTY,
    OBSTACLE
};

class Cell {
private:
    CellType type;
    Position position;

public:
    Cell();
    Cell(int x, int y, CellType type = CellType::EMPTY);
    
    CellType getType() const;
    Position getPosition() const;
    bool isPassable() const;
    bool isEmpty() const;
};

#endif