#ifndef CELL_HPP
#define CELL_HPP
#include "position.hpp"

enum class Cell_type{
    EMPTY,
    BARRICADE
};

class Cell{
    private:
        Cell_type type;
        Position position;
    public:
        Cell(Cell_type cell_type, Position cell_position);

        bool is_accessible() const;
        Cell_type get_type() const;
        Position get_position() const;

        Cell(): Cell(Cell_type::EMPTY, Position(0, 0)){}
};

#endif