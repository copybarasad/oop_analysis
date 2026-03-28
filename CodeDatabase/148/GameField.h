#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <vector>
#include "Cell.h"

/**
 * @file GameField.h
 * @brief Игровое поле
 */
class GameField {
private:
    std::vector<std::vector<Cell>> grid;
    int width;
    int height;
    
    void validateCoordinates(int x, int y) const;

public:
    GameField(int fieldWidth = DEFAULT_FIELD_SIZE, int fieldHeight = DEFAULT_FIELD_SIZE);
    
    GameField(const GameField& other);
    GameField(GameField&& other) noexcept;
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other) noexcept;
    
    bool isCellPassable(int x, int y) const;
    
    Cell& getCell(int x, int y);
    const Cell& getCell(int x, int y) const;
    
    int getWidth() const noexcept { return width; }
    int getHeight() const noexcept { return height; }
    
    void generateBlockedCells(int blockedCount);
    
    /**
     * @brief Установить состояние блокировки клетки
     * @param x Координата X
     * @param y Координата Y
     * @param blocked true если заблокирована
     */
    void setCellBlocked(int x, int y, bool blocked);
    
    const std::vector<std::vector<Cell>>& getGrid() const { return grid; }
};

#endif // GAMEFIELD_H


