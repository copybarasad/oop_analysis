#pragma once

#include <fstream>
#include <filesystem>
#include "json.hpp"
#include "world.hpp"

using json = nlohmann::json;

class Game;

class SaveManager{
private:
    std::filesystem::path save_dir = "saves";
public:

    std::filesystem::path get_save_dir();

    int save_game(bool is_active, bool is_player_turn, bool friendly_fire, int cooldown_build, int cooldown_tower, World* world, int level);    
    Game load_game(Game copy_game);
    bool valid_check_json(json load_json);
};