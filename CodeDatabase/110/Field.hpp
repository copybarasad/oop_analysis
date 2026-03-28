#ifndef FIELD_H
#define FIELD_H

#include <iostream>

#include <random>
#include <vector>
#include "../Cell/Cell.hpp"

class Field {
    private:
    unsigned int width, height;
    std::vector<std::vector<Cell>> map;

    public:
    Field (unsigned int widthValue, unsigned int heightValue, double blockedPercent=0, double slowedPercent=0);
    Field (const Field& other);
    Field (Field&& other);

    Field& operator=(const Field& other);
    Field& operator=(Field&& other);

    unsigned int getWidth () const;
    unsigned int getHeight () const;
    Cell& getCell (size_t x, size_t y);
    const Cell& getCell (size_t x, size_t y) const;

    private:
    void initializeMap (double blockedPercent, double slowedPercent);
    void generateBlockedCells (double blockedPercent);
    void generateSlowedCells (double slowedPercent);
    void copyField(const Field& other);
    void moveField(Field&& other);
};

#endif