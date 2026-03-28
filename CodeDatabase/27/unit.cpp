#include "unit.h"
#include <cstdlib>

Unit::Unit(Grid* grid, int hp, int damage, bool friendly, int x, int y) {
    this->grid = grid;
    this->hp = hp;
    this->damage = damage;
    this->friendly = friendly;
    this->x_pos = x;
    this->y_pos = y;
}

bool Unit::is_adjacent_position(Point* target_point) const {
    if (current_point == nullptr) {
        return true;
    }

    int x = target_point->x_position();
    int y = target_point->y_position();

    int dx = std::abs(x - x_pos);
    int dy = std::abs(y - y_pos);

    return (dx <= 1 && dy <= 1 && !(dx == 0 && dy == 0));
}

void Unit::go_to(Point* target_point) {
    if (!target_point) {
        return;
    }
    if (!grid) {
        return;
    }
    
    int target_x = target_point->x_position();
    int target_y = target_point->y_position();

    if (!target_point->is_passable()) {
        return;
    }
    
    if (!is_adjacent_position(target_point)) {
        return;
    }

    if (target_point->is_occupied()) {
        on_move_to_point(*target_point);
        return; 
    }

    on_move_to_point(*target_point); 

    if (current_point != nullptr) {
        current_point->set_occupier(nullptr);
    }
    
    x_pos = target_x;
    y_pos = target_y;
    current_point = target_point;
}
