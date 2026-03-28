#include "cell.h"

Cell::Cell(Cell_type cell_type, Position cell_position) 
    : type(cell_type), position(cell_position) {}

bool Cell::is_accessible() const {
    return type == Cell_type::EMPTY;
}

Cell_type Cell::get_type() const {
    return type;
}

Position Cell::get_position() const {
    return position;
}