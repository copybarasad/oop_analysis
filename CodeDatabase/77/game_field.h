#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include <vector>
#include <iostream>

#include "cell.h"
    
class GameField {

private:
    int sizeX;
    int sizeY;
    std::vector<std::vector<Cell>> field;
    
public:

    GameField(int sizeX, int sizeY);

    GameField(const GameField& other);
    
    GameField(GameField&& other) noexcept;
    
    GameField& operator=(const GameField& other);
    
    GameField& operator=(GameField&& other) noexcept;

    void reinitialize(int newWidth, int newHeight);

    int getSizeX() const;
    int getSizeY() const;
    Cell& getCell(int y, int x) const;

    void setCellType(int y, int x, CellType celltype);

    void showField() const;
};


#endif