#ifndef FIELD_H
#define FIELD_H

#include "Cell.h"
#include <vector>
#include <stdexcept>

class Field {
public:
    Field(int width, int height);
    Field(const Field& other);
    Field& operator=(const Field& other);

    int getWidth() const;
    int getHeight() const;
    bool isInside(int x, int y) const;

    Cell& getCell(int x, int y);
    const Cell& getCell(int x, int y) const;
    Cell::Type getCellType(int x, int y) const;
    void setCellType(int x, int y, Cell::Type type);

    ~Field();

private:
    int width_;
    int height_;
    std::vector<Cell> grid_;
};

#endif // FIELD_H
