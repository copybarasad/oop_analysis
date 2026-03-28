#pragma once

#include "hero.hpp"
#include "level.hpp"
#include <optional>
#include <string>

#include "gamecontroller.hpp"
#include "gamevisualizer.hpp"
#include "consoleinput.hpp"
#include "consolerenderer.hpp"

class GameEngine {
private:
    Hero player;
    int current_level_num = 1;
    bool game_over = false;
    bool game_won = false;
public:
    std::optional<Level> current_level_instance;
    GameEngine();
    void run();
    void show_main_menu();
    void show_save_menu();
    bool start_new_game();
    bool load_game(const std::string& filename);
    bool save_game(const std::string& filename);
    void run_level_loop();
    void load_next_level();
    void upgrade_player();
    int get_current_level() const { return current_level_num; }
    void set_current_level(int level) { current_level_num = level; }
    const Hero& get_player() const { return player; }
    void set_player(Hero&& h) { player = std::move(h); }
};