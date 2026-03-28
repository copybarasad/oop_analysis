#include "Combatant.h"
#include "GameArea.h"

void Combatant::generate_move(GameArea& gamearea)
{
    int search_radius = 1;
    std::vector<Coords> free_cells_nearby = gamearea.find_in_radius(coords, search_radius, { "Empty" });
    std::vector<Coords> possible_moves;
    for (auto coord : free_cells_nearby) {
        if (coord.x != coords.x && coord.y != coords.y) {
            continue;
        }
        else {
            possible_moves.push_back(coord);
        }
    }

    if (!possible_moves.empty()) {
        int index = rand() % possible_moves.size();
        Coords new_pos = possible_moves[index];
        gamearea.move_human(coords, new_pos, this);
        set_coords(new_pos.x, new_pos.y);
    }
}
