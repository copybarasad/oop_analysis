#include "move_sys.hpp"
#include "game_objects/entity.hpp"
#include "game_world/position.hpp"
#include <iostream>

void Mover::Move(Entity* e, int dx, int dy, std::vector<Position> free_cells, std::vector<Position> e_poses) {
    Position old_pos = e->getPos();
    Position new_pos = {
        old_pos.x += dx, 
        old_pos.y += dy
    };

    bool can_move = false;
    for (auto pos : free_cells) {
        if (new_pos.x == pos.x && new_pos.y == pos.y) {
            can_move = true;
            break;
        }
    }

    for (auto pos : e_poses) {
        if (new_pos.x == pos.x && new_pos.y == pos.y) {
            can_move = false;
            break;
        }
    }

    if (can_move) {
        e->setPos(new_pos.x, new_pos.y);
    }
}
