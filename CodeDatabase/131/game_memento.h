#ifndef OOP_GAME_MEMENTO_H_
#define OOP_GAME_MEMENTO_H_

#include "position.h"
#include "player.h"
#include "enemy.h"
#include "enemy_building.h"
#include "trap.h"
#include "ally.h"
#include "enemy_tower.h"
#include <vector>
#include <utility>

namespace rpg {


struct GameMemento {
    int turn_number;
    int current_level;

    size_t field_width;
    size_t field_height;

    int player_health;
    int player_max_health;
    int player_melee_damage;
    int player_ranged_damage;
    int player_score;
    Position player_position;
    CombatMode player_combat_mode;
    int player_slowed_turns;

    std::vector<int> spell_types;

    std::vector<std::pair<size_t, size_t>> field_impassable_cells;
    std::vector<std::pair<size_t, size_t>> field_slowing_cells;

    std::vector<Enemy> enemies;
    std::vector<EnemyBuilding> buildings;
    std::vector<Trap> traps;
    std::vector<Ally> allies;
    std::vector<EnemyTower> towers;
};

}

#endif

