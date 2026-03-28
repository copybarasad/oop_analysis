#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <stdexcept>
#include "cell.h"
#include "direction.h"

class Field {
private:
    std::vector<std::vector<Cell*>> grid;
    int width;
    int height;

public:
    Field(int width=20, int height=20);
    ~Field();

    Field(const Field& obj);
    Field& operator=(const Field& obj);
    Field(Field&& obj) noexcept;
    Field& operator=(Field&& obj) noexcept;

    const Cell* getCell(int x, int y) const;

    int getWidth() const;
    int getHeight() const;
    
    int isSafeWallPosition(int x, int y) const;
    void generateWalls();

    bool canMoveTo(int x, int y) const;
    bool isValidPosition(int x, int y) const;
    void setTrapCell(int x, int y,int dmg, CellType newType);

    std::vector<std::tuple<int, int, CellType, int>> getCellsData() const;
    void loadCellsData(const std::vector<std::tuple<int, int, CellType, int>>& data);

};