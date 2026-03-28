#include "Entity.h"
#include <iostream>

void Entity::go_to(Cell& cell) {
    if (!cell.is_passable()) {
        std::cout << "Cell is unpassable\n";
        return;
    }
    if (current_cell != nullptr) {
        current_cell->set_player(nullptr);
        current_cell->set_enemy(nullptr);
    }
    x_pos = cell.get_x();
    y_pos = cell.get_y();
    current_cell = &cell;
}
