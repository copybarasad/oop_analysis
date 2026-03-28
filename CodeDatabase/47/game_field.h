#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include "cell_type.h"
#include <vector>

class GameField {
private:
    int width;
    int height;
    std::vector<std::vector<CellType>> grid;

public:
    GameField(int width, int height);
    GameField(const GameField& other);
    GameField(GameField&& other) noexcept;
    
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other) noexcept;
    
    bool isValidPosition(int x, int y) const;
    bool isPassable(int x, int y) const;
    int getWidth() const;
    int getHeight() const;
    CellType getCellType(int x, int y) const;
    
    void setCellType(int x, int y, CellType type);
    void resize(int newWidth, int newHeight);
};

#endif