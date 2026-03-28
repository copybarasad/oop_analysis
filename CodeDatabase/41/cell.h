#ifndef CELL_H
#define CELL_H

#include "enums.h"

class Cell{
private:
    Cell_type type;
    bool has_player = false;
    bool has_enemy = false;
    bool has_building = false;
    bool has_ally = false;

public:
    Cell();
    Cell(Cell_type cell_type);

    Cell_type get_type() const;
    bool get_has_player() const;
    bool get_has_enemy() const;
    bool get_has_building() const;
    bool is_passable() const;

    void set_type(Cell_type cell_type);
    void set_has_player(bool has_player);
    void set_has_enemy(bool has_enemy);
    void set_has_building(bool has_building);

    bool is_empty() const;
    bool can_move_to() const;

    bool get_has_ally() const;          
    void set_has_ally(bool has_ally);
};

#endif