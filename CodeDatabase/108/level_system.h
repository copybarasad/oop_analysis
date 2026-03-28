#ifndef OOP_LEVEL_SYSTEM_H
#define OOP_LEVEL_SYSTEM_H

#include "game_state.h"
#include "field.h"

struct LevelConfig {
    int level_number;
    int field_width;
    int field_height;
    int enemy_count;
    int spawner_count;
    int tower_count;
    int enemy_health;
    int enemy_damage;
    int spawner_health;
    int tower_health;
    int tower_damage;
    int tower_range;
};

class LevelSystem {
    std::vector<LevelConfig> levels;
    int current_level_index;

    void initialize_levels();
    Position generate_random_position(const GameField& field);

public:
    LevelSystem();

    void load_level(int level_number, GameState& game_state, Player& player);

    bool is_level_completed(const GameState& game_state) const;

    int get_current_level() const;

    bool next_level(GameState& game_state, Player& player);

    bool has_next_level() const;

    void set_current_level(int level);

};



#endif //OOP_LEVEL_SYSTEM_H