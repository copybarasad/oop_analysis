#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include "cell.h"
#include "position.h"
#include "game_constants.h"
#include <vector>
#include <memory>
#include <iostream>

class GameField {
public:
    GameField(int width = GameConstants::DEFAULT_FIELD_SIZE,
        int height = GameConstants::DEFAULT_FIELD_SIZE);

    GameField(const GameField& other);
    GameField(GameField&& other) noexcept;

    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other) noexcept;

    int getWidth() const;
    int getHeight() const;
    CellType getCellType(const Position& position) const;

    bool isValidPosition(const Position& position) const;
    bool isPositionEmpty(const Position& position) const;

    void setCellType(const Position& position, CellType type);
    void clearCell(const Position& position);

    void save(std::ostream& out) const;
    void load(std::istream& in);

private:
    int fieldWidth;
    int fieldHeight;
    std::vector<std::vector<Cell>> grid;

    void initializeGrid();
    void copyFrom(const GameField& other);
    void moveFrom(GameField&& other) noexcept;
};

#endif


