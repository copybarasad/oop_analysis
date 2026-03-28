#pragma once

// #include "field_handler.hpp"
#include "game_action_handler.hpp"
#include "level_generator.hpp"
#include "game_image.hpp"

#include "zap_spell.hpp"
#include "fireball_spell.hpp"
#include "trap_spell.hpp"
#include "summon_spell.hpp"

#include "screen.hpp"

enum GameState {
    VICTORY,
    DEFEAT,
    IN_PROCESS,
};

class Game
{
private:
    int score = 0;
    int coins = 10;
    int time = 0;
    int lvl = 1;

    int cap_lvl = 0;
    int hp_lvl = 0;

    Level lvl_gen;

    FieldHandler field_handler;
    GameActionHandler action_handler;

    bool handle_menu_action();

    int open_store();

    void player_move(const Position& pos);
    void ally_move(const Position& pos);
    void enemy_move(const Position& pos);
    void enemy_spawner_move(const Position& pos);
    void enemy_tower_move(const Position& pos);

    void tick();
public:
    Game();
    Game(GameImage& img);

    GameImage get_data_image();

    GameState game_state();

    GameState start();
    void restart_lvl();
    void next_lvl();
};