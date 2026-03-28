#pragma once

#include <functional>
#include <algorithm>

#include "world.hpp"
#include "save_manager.hpp"
#include "command_manager.hpp"

#define BASE_FRIENDLY_FIRE true
#define BASE_COOLDOWN_TOWER 5

class Game{
private:
    bool is_active{true};
    bool is_player_turn{true};
    bool friendly_fire{true};
    int cooldown_build{0};
    int cooldown_tower{BASE_COOLDOWN_TOWER}; // раз в какое время башня сможет ходить
    World* world;
    int level{1};

    SaveManager sm;
public:
    Game(int size_x = MIN_SIZE, int size_y = MIN_SIZE, Player* player_ptr = nullptr,
        int cooldown_tower = BASE_COOLDOWN_TOWER, bool friendly_fire = BASE_FRIENDLY_FIRE);
    Game(World* world_ptr, bool active, bool player_turn, bool friendly_fire, 
        int build_cooldown, int tower_cooldown);
    Game(const Game& other);
    Game& operator=(const Game& other);
    Game& operator=(Game&& other) noexcept;
    ~Game();

    void init_world(int cnt_enemy, int cnt_enemy_build, int cnt_enemy_tower, int required_quantity, float coef);

    template<typename Controller, typename Visualizer>
    void run(Controller& controller, Visualizer& visualizer, int cnt_enemy,
        int cnt_enemy_build, int cnt_enemy_tower, int required_quantity);

    void process_enemy();

    bool is_friendly_fire();

    void gen_new_level(World* snapshot_world, int cnt_enemy, int cnt_enemy_build, int cnt_enemy_tower, int required_quantity);

    void set_level(int nl);
    int get_level();

    World* get_world();

    int handle_command(CommandManager& cmd);
};

#include "game.tpp"