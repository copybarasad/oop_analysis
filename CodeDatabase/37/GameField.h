#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include "Position.h"
#include <vector>

enum class CellType { EMPTY, BLOCKED };

class GameField {
private:
    int width, height;
    std::vector<std::vector<CellType>> grid;

public:
    GameField(int w, int h);
    
    GameField(const GameField& other);
    GameField(GameField&& other);
    
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other);
    
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    bool isCellPassable(const Position& pos) const;
    CellType getCellType(const Position& pos) const;
    
private:
    void generateBlockedCells();
};

#endif