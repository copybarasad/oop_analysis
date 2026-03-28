#ifndef FIELD_H
#define FIELD_H

#include "Cell.hpp"
#include "Direction.hpp"
#include <vector>
#include <iostream>

class Field {
private:
    int width_;
    int height_;
    std::vector<std::vector<Cell>> cells_;
    
    void initializeCells();
    void placeImpassableCells();
    void placeSlowingCells();

public:
    Field();
    Field(int width, int height, int startX, int startY);
    
    Field(const Field& other);
    Field(Field&& other) noexcept;
    Field& operator=(const Field& other);
    Field& operator=(Field&& other) noexcept;

    bool isValidPosition(int x, int y) const;
    int getWidth() const;
    int getHeight() const;
    const Cell& getCell(int x, int y) const;
    Cell& getCell(int x, int y);
    
    Direction getRandomDirection() const;

    void serialize(std::ostream& os) const;
    void deserialize(std::istream& is);
};

#endif