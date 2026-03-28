#include "EnemyBuilding.h"
#include "GameArea.h"
#include <vector>

void EnemyBuilding::do_turn(GameArea& gamearea)
{
	step++;
    if (step == count_step_spawn) {
        spawn_enemy(gamearea);
        step = 0;
    }
}

Coords EnemyBuilding::find_nearest_free_cell(GameArea& gamearea) {

    const std::vector<Coords> directions = {
        Coords(1, 0),   // право
        Coords(0, 1),   // вверх
        Coords(-1, 0),  // лево
        Coords(0, -1),  // вниз
        Coords(1, 1),   // право-вверх
        Coords(-1, 1),  // лево-вверх
        Coords(1, -1),  // право-вниз
        Coords(-1, -1)  // лево-вниз
    };

    for (auto dir : directions) {
        Coords candidate = coords + dir;
        if (gamearea.can_move_to(candidate) == "Empty" && gamearea.get_cell_type(candidate) != CellType::BLOCKED) {
            return candidate;
        }
    }

    return coords;
}

void EnemyBuilding::spawn_enemy(GameArea& gamearea) {
    Coords spawn_pos = find_nearest_free_cell(gamearea);
    if (spawn_pos != coords) {
        Enemy* new_enemy = new Enemy(10, 5, spawn_pos);
        gamearea.set_human_in_cell(spawn_pos, new_enemy);

    }
}

