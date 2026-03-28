#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include <vector>
#include "Position.h"
#include "GameCell.h"

class GameField
{
private:
    std::vector<std::vector<GameCell>> grid;
    int width;
    int height;

public:
    GameField(int fieldWidth, int fieldHeight);
    int getWidth() const;
    int getHeight() const;
    bool isValidPosition(const Position &position) const;
    const GameCell &getCell(const Position &position) const;
    bool isPositionPassable(const Position &position) const;
    void setCellOccupied(const Position &pos, bool occ);
    void setCellType(const Position& position, CellType type);
    void initializeFromData(int fieldWidth, int fieldHeight, 
                           const std::vector<CellType>& cellTypes);
};

#endif