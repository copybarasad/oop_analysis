#ifndef CELL_H
#define CELL_H

#include "character.h"

enum class cell_type{ 
    usual,      // obi4naya             _
    delay,      // zamedlyayuschtschaya ~
    impassable  // neprohodimaya        x
};              // trap                 #

class Cell{
    public: 
    cell_type type;
    bool trap;
    unsigned int damage;
    Character* occupied;

    Cell();

    Cell(cell_type type, Character* occupied);

    Cell(const Cell& cell);

    Cell& operator=(const Cell& cell);

    Cell(Cell&& cell) noexcept;

    Cell& operator=(Cell&& cell) noexcept;

    ~Cell();

    void set_trap(unsigned int damage);

    unsigned int use_trap();
};

#endif