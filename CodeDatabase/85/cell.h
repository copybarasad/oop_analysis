#ifndef CELL_H
#define CELL_H

#include "position.h"

enum class Cell_type {
    EMPTY,
    OBSTACLE
};

class Cell {
private:
    Cell_type type;
    Position position;

public:
    Cell(Cell_type cell_type, Position cell_position);
    bool is_accessible() const;
    Cell_type get_type() const;
    Position get_position() const;
};

#endif