#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include "Cell.h"
#include "Position.h"
#include "GameTypes.h"

#include <vector>
#include <random>

class GameField {
public:
    GameField(int width = 15, int height = 15);

    GameField(const GameField& other);
    GameField& operator=(const GameField& other);
    GameField(GameField&& other) noexcept;
    GameField& operator=(GameField&& other) noexcept;

    ~GameField() = default;

    int getWidth() const;
    int getHeight() const;

    CellType getCellType(int x, int y) const;
    bool isCellPassable(int x, int y) const;
    const Cell& getCell(int x, int y) const;

    void setCellType(int x, int y, CellType type);

    bool isValidPosition(int x, int y) const;

    const std::vector<std::vector<Cell>>& getGrid() const;

    Pos findFirstPassableCell() const;
    
    // Save/Load methods
    std::vector<std::vector<int>> getCellTypesAsInt() const;
    void restoreFromCellTypes(const std::vector<std::vector<int>>& types);

private:
    int width_;
    int height_;
    std::vector<std::vector<Cell>> grid_;

    mutable std::mt19937 rng_;

    void generateBlockedCells();
    void generateSlowingCells();
};

#endif