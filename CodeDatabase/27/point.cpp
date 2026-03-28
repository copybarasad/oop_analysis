#include "point.h"
#include "player.h"
#include "enemy.h"
#include "grid.h"

void Point::set_occupier(Unit* unit) { occupier = unit; }
Unit* Point::get_occupier() const { return occupier; }

void Point::set_passable(bool is) { passable = is; }
bool Point::is_passable() const { return passable; }

void Point::set_x(int x) { x_pos = x; }
void Point::set_y(int y) { y_pos = y; }
int Point::x_position() const { return x_pos; }
int Point::y_position() const { return y_pos; }

bool Point::is_occupied() const { return occupier != nullptr; }

bool Point::is_occupied_by_player() const {
    return occupier && dynamic_cast<Player*>(occupier);
}

bool Point::is_occupied_by_enemy() const {
    return occupier && dynamic_cast<Enemy*>(occupier);
}

void Point::set_parent_grid(Grid* grid) { parent_grid = grid; }
Grid* Point::get_parent_grid() const { return parent_grid; }

void Point::place_trap(int damage) { trap_damage = damage; }
int Point::pop_trap_damage() {
    int d = trap_damage;
    trap_damage = 0;
    return d;
}

