#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <vector>
#include <string>
#include <iostream>
#include "Cell.h"
#include "Position.h"
#include "../exceptions/GameException.h"

class GameField {
    std::vector<std::vector<Cell>> grid;
    int width;
    int height;
    
public:
    GameField(int w = 15, int h = 15);
    GameField(const GameField& other);
    GameField(GameField&& other) noexcept;
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other) noexcept;
    
    void initializeBlockedCells();
    
    bool isValidPosition(const Position& pos) const;
    bool isCellFree(const Position& pos) const;
    bool isCellBlocked(const Position& pos) const;
    
    void setPlayerPosition(const Position& pos);
    void setEnemyPosition(const Position& pos);
    void setTowerPosition(const Position& pos);
    void setTrapPosition(const Position& pos);
    void removeTrapPosition(const Position& pos);
    void clearCell(const Position& pos);
    
    Cell getCell(const Position& pos) const;
    
    int getWidth() const;
    int getHeight() const;
    
    std::string serialize() const;
    void deserialize(std::istream& stream);
};

#endif