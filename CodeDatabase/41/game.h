#ifndef GAME_H
#define GAME_H

#include "game_field.h"
#include "player.h"
#include "hand.h"
#include <string>
#include <memory>

class Game{
private:
    std::unique_ptr<Game_field> field;
    std::unique_ptr<Player> player;
    int current_level;
    bool game_running;
    int total_turns;

    void game_loop();
    void init_level();
    void player_turn();
    void allies_turn();
    void enemies_turn();
    void buildings_turn();
    bool check_win_condition() const;
    bool check_lose_condition() const;
    void remove_half_spells();

public:
    Game();
    ~Game() = default;
    Game(Game&& other) noexcept;
    Game& operator=(Game&& other) noexcept;
    void start();
    void start_new_game();
    bool load_game(const std::string& file_name);
    bool save_game(const std::string& file_name) const;
    void next_level();
    void show_level_up_menu();
    void game_over();

    int get_current_level() const;
    int get_total_turns() const;
    Player* get_player() const;
    Game_field* get_field() const;
    
    void set_current_level(int level);
    void set_total_turns(int turns);

    void initialize_objects(int width = MIN_FIELD_SIZE, int height = MIN_FIELD_SIZE);

    // для 4 лр
    bool is_running() const;
    void update();
    void stop_game();
    
    // Методы для обработки команд из Input_handler
    void handle_move(int dx, int dy);
    void handle_attack();
    void handle_switch_mode();
    void handle_heal();
    void handle_cast_spell(int spell_index, int target_x, int target_y);
    void handle_show_spells();
    void handle_learn_spell();
    void handle_save(const std::string& filename = "autosave.txt");
    void handle_load(const std::string& filename = "autosave.txt");
    void handle_quit();

    // для 4 лр
    void start_external();  
    void external_update();
    void start_new_game_external();
    void restart_with_new_system();
};

#endif