#include "cell.h"

Cell::Cell(CellType type) :
    type(type), passable(type != CellType::OBSTACLE), trap_damage(0) {}

CellType Cell::get_type() const {
    return type;
}

bool Cell::is_passable() const {
    return passable;
}

void Cell::set_type(CellType new_type) {
    type = new_type;
    passable = (new_type != CellType::OBSTACLE);
}


void Cell::set_trap_damage(int damage) {
    trap_damage = damage;
}

int Cell::get_trap_damage() const {
    return trap_damage;
}









