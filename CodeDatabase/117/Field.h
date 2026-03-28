#ifndef FIELD_H
#define FIELD_H

#include "Cell.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

class Character;

class Field{
private:
    Cell** cells;
    int width;
    int height;

public:
    Field();
    Field(int width, int height);
    Field(const Field& other);
    Field& operator = (const Field& other);
    Field(Field&& other);
    Field& operator= (Field&& other);
    ~Field();

    int getWidth() const;
    int getHeight() const;
    bool isValidPos(int x, int y) const;
    bool isValidPos(std::pair<int, int> position) const;
    bool canMoveTo(std::pair<int, int> position) const;
    bool canMoveTo(int x, int y) const;
    Cell& getCell(int x, int y);
    Cell& getCell(std::pair<int, int> position);

    std::pair<int, int> freePosAround(std::pair<int, int> position) const;
    void setTypeToCells(std::pair<int, int> point1, std::pair<int, int> point2, CellType type);
};

#endif