#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include "cell.h"

class Field{
private:
    int width;
    int height;
    std::vector<std::vector<Cell>> grid; //двумерный динам масс grid[x][y] клетка соотв
public:
    Field(int width, int height);
    // важное для масс
    Field(const Field& other); //cop
    Field& operator=(const Field& other);
    Field(Field&& other) noexcept; //tran
    Field& operator=(Field&& other) noexcept;

    int GetWidth() const;
    int GetHeight() const;
    Cell& GetCell(int x, int y);
    const Cell& GetCell(int x, int y) const;
    void AddBlockCell(double chance);
    void Print() const;
};

#endif