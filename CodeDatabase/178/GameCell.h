#ifndef GAME_CELL_H
#define GAME_CELL_H

#include "CellType.h"

class GameCell
{
private:
    CellType type;
    bool isOccupied;

public:
    GameCell(CellType cellType = CellType::EMPTY);
    CellType getType() const;
    bool getIsOccupied() const;
    bool isPassable() const;
    void setOccupied(bool occ) { isOccupied = occ; }
};

#endif