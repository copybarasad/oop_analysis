#pragma once
#include "GameArea.h"
#include "Player.h"
#include "Enemy.h"
#include "Summon.h"
#include "Tower.h"
#include "EnemyBuilding.h"
#include "GameState.h"
#include "SaveManager.h"
#include <vector>
#include <algorithm>
#include "IGameView.h"
#include <iostream>

class Game {
private:
    GameArea* gamearea = nullptr;
    Player* player = nullptr;
    std::vector<Enemy*> enemies;
    std::vector<Summon*> summons;
    std::vector<Building*> buildings;

    int current_level = 1;
    bool game_running = true;

    IGameView* view = nullptr;

public:
    Game() : current_level(1), game_running(true), gamearea(nullptr), player(nullptr) {}
    ~Game() {
        delete gamearea;
        delete player;
        for (auto e : enemies)
            delete e;
        enemies.clear();
        for (auto s : summons)
            delete s;
        summons.clear();
        for (auto b : buildings)
            delete b;
        buildings.clear();
    }

    void set_view(IGameView* new_view);
    void start_new_game();
    void init_level();

    void move_player(std::string dir);
    void cast_spell_mode();
    void save_game_command();
    void stop_game();
    bool is_running() const;

    GameArea* get_area();
    Player* get_player();

    void update_world();

    void allies_turn();
    void enemies_turn();
    void buildings_turn();

    void check_allies_on_area();
    bool check_win_condition();
    bool check_lose_condition();

    void upgrade_player();
    void next_level();
    void handle_game_over();

    void save_current_game();
    void load_saved_game();
};
