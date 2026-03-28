#pragma once
#include <vector>
#include "Gamefield.h"
#include "Player.h"
#include "Enemy.h"
#include "GameState.h"
#include "EnemyTower.h"
#include "Hand.h"
#include "DirectAttackSpell.h"
#include "AoeAttackSpell.h"
#include "TrapSpell.h"
#include "Spell.h"

class Game {
private:
    GameState current_state;
    std::unique_ptr<Gamefield> field;
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::unique_ptr<EnemyTower> enemy_tower;
    Hand hand;


    // Меню
    void show_main_menu();
    void start_new_game();
    void game_over();

    // Уровни, инициализация, прокачка
    void level_complete();
    void level_init();
    void reset_game_state();
    void level_upgrade();

    // Основной цикл уровня
    void process_game_loop();
    void check_game_over();
    void remove_dead_enemies();
    void process_inputs(char& input, const std::vector<std::unique_ptr<Spell>>& spells, int* idx);

    // Save / Load
    unsigned long long calculate_hash(const std::string& data);
    void load_gamefile();
    void load_game();
    void save_game();

    int current_level;
    static constexpr int DEFAULT_FIELD_SIZE = 10;
    static constexpr int MAX_LEVELS = 3;
    static constexpr int MIN_ENEMY_DMG = 10;
    static constexpr const char* SAVE_FILE_NAME = "game_save.dat";
public:
    Game();
    ~Game();
    void Run();
};
