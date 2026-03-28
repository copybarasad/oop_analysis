#ifndef CELL_H
#define CELL_H

#include <fstream>
#include <cstdio>
#include "cell_type.h"

class Cell
{
private:
    CellType type;
    bool is_available;
    bool is_slow_trap;
    bool is_trap_spell;

public:
    Cell();
    
    CellType get_type() const;
    bool get_availability() const;
    bool get_slow_trap() const;
    bool get_trap_spell() const;

    void set_type(CellType new_type);
    void set_availability(bool availability);
    void set_slow_trap(bool slow_trap);
    void set_trap_spell(bool trap_spell);

    void save_binary(std::ostream& os) const;
    void load_binary(std::istream& is);
};

#endif