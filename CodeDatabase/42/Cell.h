#pragma once
#include "Position.h"

enum class CellType {
    EMPTY,
    PLAYER,
    ENEMY,
    OBSTACLE,
    TOWER,
    TRAP
};

class Cell {
private:
    Position position;
    CellType type;

public:
    Cell(int x = 0, int y = 0);

    Position getPosition() const;
    CellType getType() const;
    void setType(CellType newType);
    bool isPassable() const;
};
