#ifndef GAME_CELL_H
#define GAME_CELL_H

#include "game_enums.h"

class GameCell {
private:
    CellType type;
    bool isOccupied;

public:
    GameCell(CellType cellType = CellType::EMPTY);
    
    CellType getType() const;
    void setType(CellType cellType);
    bool getIsOccupied() const;
    void setIsOccupied(bool occupied);
    bool isPassable() const;
};

#endif