#include <random>
#include <algorithm>
#include <set>

// #include "field.hpp"

#include "field_handler.hpp"

#include "game_object.hpp"
#include "position.hpp"

#include "stone.hpp"
#include "swamp.hpp"

class Level {
private:
    int seed = 9;
    std::mt19937 gen;

public:
    Level() = default;

    FieldHandler get_lvl(int lvl);
private:
    void gen_landscape(FieldHandler& field_handler);
    void gen_entities(FieldHandler& field_handler, int lvl);

    std::set<Position> gen_positions(std::vector<Position> free_cells, int count);

    Position gen_player_pos(FieldHandler& field_handler, int difficulty);
    std::set<Position> gen_enemies_pos(FieldHandler& field_handler, int difficulty);
    std::set<Position> gen_enemy_towers_pos(FieldHandler& field_handler, int difficulty);
    std::set<Position> gen_enemy_spawners_pos(FieldHandler& field_handler, int difficulty);

    void place_enemies(FieldHandler& field_handler, const std::set<Position>& positions, const MobImage& img);
    void place_enemy_towers(FieldHandler& field_handler, const std::set<Position>& positions, const BuildingImage& img);
    void place_enemy_spawners(FieldHandler& field_handler, const std::set<Position>& positions, const BuildingImage& img);
};