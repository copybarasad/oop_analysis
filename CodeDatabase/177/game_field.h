#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include "game_cell.h"
#include "position.h"
#include <vector>

class GameField {
private:
    int width;
    int height;
    std::vector<std::vector<GameCell>> grid;
    
    void initializeField();
    bool isValidPosition(const Position& pos) const;

public:
    GameField(int fieldWidth = 15, int fieldHeight = 15);
    GameField(const GameField& other);
    GameField(GameField&& other) noexcept;
    
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other) noexcept;
    
    int getWidth() const;
    int getHeight() const;
    
    const GameCell& getCell(const Position& pos) const;
    bool isPositionPassable(const Position& pos) const;
    void setCellOccupied(const Position& pos, bool occupied);
    void setCellType(const Position& pos, CellType type);
    void clearAllOccupancy();
    std::vector<Position> getAdjacentPositions(const Position& pos) const;
};

#endif