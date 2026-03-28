#pragma once
#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include "Field.h"
#include "PlayerHand.h"
#include <vector>
#include <string>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

class GameManager {
private:
    PlayerHand* playerHand;
    Player* player;
    Field* field;
    std::vector<Enemy> enemies;
    std::vector<EnemyTower> towers;
    Game* game;
    int current_level = 1;

public:
    GameManager() : playerHand(nullptr), player(nullptr), field(nullptr), game(nullptr) {}
    ~GameManager() { clean_up(); }

    void run_menu();
    void start_new_game();

    bool save_game(std::string filename);
    bool load_game(std::string filename);

    bool validate_save_json(json& j);
private:
    void clean_up();
    void game_loop();

    void setup_level(int level);
    void level_up_menu();
    void clear_level_resources();
};
