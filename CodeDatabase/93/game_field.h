#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include <vector>
#include "cell.h"
#include "trap.h"
#include <iostream>

class EnemyTower;

class GameField {
private:
    std::vector<std::vector<Cell>> grid;
    std::vector<Trap> traps_;
    int width;
    int height;

    void initializeGrid();

public:
    GameField(int width, int height);
    GameField();
    GameField(const GameField& other) = delete;
    GameField& operator=(const GameField& other) = delete;
    GameField(GameField&& other) noexcept = default;
    GameField& operator=(GameField&& other) noexcept = default;

    void resizeEmpty(int w, int h);

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    const Cell& getCell(int x, int y) const;
    const std::vector<Trap>& getTraps() const { return traps_; }
    std::vector<Trap>& getTraps() { return traps_; }

    bool isPositionValid(int x, int y) const;
    bool isPositionPassable(int x, int y) const;
    bool isPositionEmpty(int x, int y) const;

    void addTrap(const Trap& trap);
    void removeInactiveTraps();

    bool isTowerAt(int x, int y, const std::vector<EnemyTower>& towers) const;

    void save(std::ostream& os) const;
    void load(std::istream& is);
};

#endif