#pragma once

#include <vector>
#include <memory>

#include "Cell.h"
#include "../eneties/Enemy.h"
#include "../eneties/Building.h"

class GameField {
private:
    int width;
    int height;
    std::vector<std::vector<Cell>> grid;

public:
    GameField(int w = 15, int h = 15);
    ~GameField();
    GameField(const GameField& other);
    GameField& operator=(const GameField& other);
    GameField(GameField&& other);
    GameField& operator=(GameField&& other);

    void setCell(int x, int y, Cell cell);
    int getWidth() const;
    int getHeight() const;
    const Cell& getCell(int x, int y) const;
    Cell& getCell(int x, int y);
    bool isValidMove(int x, int y) const;
    bool isStructure(int x, int y) const;
};