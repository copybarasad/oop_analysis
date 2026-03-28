#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "Position.h"
#include <vector>

enum class CellType {
    EMPTY,
    PLAYER,
    ENEMY,
    WALL,
    TRAP,
    TOWER,
    ALLY
};

class GameMap {
private:
    std::vector<std::vector<CellType>> grid;
    int width;
    int height;

    void initGrid();
    bool isValid(int x, int y) const;

public:
    GameMap(int width, int height);
    GameMap(const GameMap& other);
    GameMap(GameMap&& other) noexcept;
    
    GameMap& operator=(const GameMap& other);
    GameMap& operator=(GameMap&& other) noexcept;
    
    ~GameMap() = default;
    
    int getWidth() const;
    int getHeight() const;
    CellType getCell(int x, int y) const;
    
    void setCell(int x, int y, CellType type);
    void clearCell(int x, int y);
    
    bool isEmpty(int x, int y) const;
    bool isWall(int x, int y) const;
    bool inBounds(int x, int y) const;
    
    void display() const;
};

#endif