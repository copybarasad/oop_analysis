#ifndef OOP_GAME_H
#define OOP_GAME_H

#include "player.h"
#include "field.h"
#include "save_load_system.h"
#include "level_system.h"


class GameLoop {
    GameField field;
    GameState game_state;
    Player player;

    int current_turn;
    int current_level;
    bool game_running;
    bool need_to_skip_enemies_turn;    // флаг для отслеживания была ли нажата кнопка V - сохранить, L - load, Q - ошибочно, чтобы не тратить на них ход и не ходили противники.


    SaveLoadSystem save_load_system;
    LevelSystem level_system;


    ISpell* generate_random_spell();

    // void render_field() const;       // лб 4 - класс отрисовки (отдельный) (!), пока тут. UPD убрали


    // void process_player_turn(int current_turn);  // - лб4 - выносим.


    void process_enemies_turn();
    void process_spawners_turn();
    void process_towers_turn();


    // NEW
    bool check_level_completed() const;
    bool check_player_defeated() const;

    void handle_level_completion();
    void handle_level_transition();
    void handle_game_over();
    void handle_game_win();

    void show_upgrade_menu();
    void show_restart_menu();


public:
    GameLoop();
    void run();


    const Player& get_player() const;
    int get_current_turn() const;
    const GameState& get_game_state() const;

    // вынесение логики команд отдельно
    void execute_wasd(char symbol);
    void execute_switch_combat_mode();
    void execute_kick();
    void execute_load();
    void execute_save();
    void execute_cast_spell(int spell_index);
    void execute_quit();


};

#endif //OOP_GAME_H