#pragma once

#include <vector>
#include <memory>
#include "Cell.h"
#include "Position.h"

class GameField {
private:
    std::vector<std::vector<Cell>> grid;
    int width;
    int height;

    void initializeField();

public:
    GameField(int width, int height);
    GameField(const GameField& other);
    GameField(GameField&& other) noexcept;

    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other) noexcept;

    int getWidth() const;
    int getHeight() const;

    bool isValidPosition(int x, int y) const;
    bool isCellPassable(int x, int y) const;
    bool isCellEmpty(int x, int y) const;
    bool isEnemyHere(int x, int y) const;
    CellType getCellType(int x, int y) const;

    void setPlayerPosition(const Position& position);
    void clearPlayerPosition(const Position& position);

    void setEnemyPosition(const Position& position);
    void clearEnemyPosition(const Position& position);

    void setBuildingPosition(const Position& position);
    void clearBuildingPosition(const Position& position);

    void setTowerPosition(const Position& position);
    void clearTowerPosition(const Position& position);

    void setAllyPosition(const Position& position);
    void clearAllyPosition(const Position& position);

    void setTrap(const Position& position, int damage);
    void clearTrap(const Position& position);
    bool hasTrap(int x, int y) const;
    int getTrapDamage(int x, int y) const;

    const Cell& getCell(int x, int y) const;
    Cell& getCellMutable(int x, int y);
};
