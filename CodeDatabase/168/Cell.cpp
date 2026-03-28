#include "Cell.h"

Cell::Cell(CellType type) 
    : type(type), Player_on(false), Enemy_on(false), Building_on(false), 
      Tower_on(false), Ally_on(false), trapDamage(0) {}

bool Cell::is_Passable() const {
    return type != CellType::OBSTACLE && !Building_on && !Tower_on;
}

bool Cell::is_Empty() const {
    return !Player_on && !Enemy_on && !Building_on && !Tower_on && !Ally_on && type != CellType::OBSTACLE;
}

CellType Cell::get_type() const {
    return type;
}

void Cell::set_Player(bool is_set) {
    Player_on = is_set;
}

void Cell::set_Enemy(bool is_set) {
    Enemy_on = is_set;
}

void Cell::set_Building(bool is_set) {
    Building_on = is_set;
}

void Cell::set_Tower(bool is_set) {
    Tower_on = is_set;
}

void Cell::set_Ally(bool is_set) {
    Ally_on = is_set;
}

void Cell::set_Trap(int damage) {
    type = CellType::TRAP;
    trapDamage = damage;
}

void Cell::clear_Trap() {
    if (type == CellType::TRAP) {
        type = CellType::EMPTY;
        trapDamage = 0;
    }
}

bool Cell::has_Player() const {
    return Player_on;
}

bool Cell::has_Enemy() const {
    return Enemy_on;
}

bool Cell::has_Building() const {
    return Building_on;
}

bool Cell::has_Tower() const {
    return Tower_on;
}

bool Cell::has_Ally() const {
    return Ally_on;
}

bool Cell::has_Trap() const {
    return type == CellType::TRAP;
}

int Cell::get_TrapDamage() const {
    return trapDamage;
}

void Cell::clear() {
    Player_on = false;
    Enemy_on = false;
    Building_on = false;
    Tower_on = false;
    Ally_on = false;
}
