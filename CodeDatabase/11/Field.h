#ifndef FIELD_H
#define FIELD_H

#include "Cell.h"
#include <fstream>

class EventLogger;

class Field {
public:
    Field(int width, int height);
    ~Field();
    
    Field(const Field& other);
    Field& operator=(const Field& other);
    Field(Field&& other) noexcept;
    Field& operator=(Field&& other) noexcept;

    void print() const;
    bool isValidPosition(int x, int y) const;
    CellType getCellType(int x, int y) const;
    void setCellType(int x, int y, CellType type);
    int getWidth() const;
    int getHeight() const;

    bool save(std::ofstream& out, EventLogger& logger) const;
    bool load(std::ifstream& in, EventLogger& logger);

private:
    int width;
    int height;
    Cell** grid;

    void allocateGrid();
    void deallocateGrid();
    void copyFrom(const Field& other);
};

#endif