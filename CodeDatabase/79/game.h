#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "place.h"
#include "tower.h"
#include "magic_tower.h"
#include "enemy.h"
#include "movement.h"
#include "spell_hand.h"
#include "game_controller.h"
#include "render_controller.h"

#include <vector>
#include <memory>
#include <string>
#include <cstdlib>
#include <fstream>


class SaveException : public std::exception {
private:
    std::string message;
public:
    explicit SaveException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class LoadException : public std::exception {
private:
    std::string message;
public:
    explicit LoadException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class Game {
public:
    enum class Menu{Main_menu, Level, Level_complete, Game_over, Upgrade_menu};

private:
    Menu menu;
    int current_level;
    Player player;
    Place place;
    std::vector<Enemy> enemies;
    std::unique_ptr<Tower> tower;
    MagicTower magic_tower;
    std::unique_ptr<Movement> movement;
    bool game_continue;
    Game_controller<Console_input> game_controller;
    Render_controller<Render> render_controller;

    struct Save_data{
        int level;
        int playerHealth;
        int playerMaxHealth;
        int playerDamage;
        int playerPoints;
        int playerX;
        int playerY;
        int base_x;
        int base_y;

        int tower_x;
        int tower_y;
        int towerHealth;
        int towerMaxHealth;
        int magic_tower_x;
        int magic_tower_y;
        int magicTowerHealth;
        int magicTowerMaxHealth;
        int towerspoints;
        
        int enemyCount;
        int enemyHealth[20];
        int enemyMaxHealth[20];
        int enemyDamage[20];
        int enemyX[20];
        int enemyY[20];
        int enemypoints[20];

        int spellCount;
        int spellTypes[3];
    };

    static const std::string SAVE_FILE;

public:
    Game();

    void run();
    void start_new_game();
    bool load_game();
    bool save_game();
    bool exit_game();

    void create_level(int level);
    void next_level();
    bool level_complete();
    bool game_over();

    void player_turn();
    void enemy_turn();
    void player_turn1();
    int move(Movement::Direction dir);
    int attack();
    int use_spell();
    int buy_spell();
    int change_weapon();


    void upgrade_menu();
    void upgrade(char choice);


    void delete_game();
    // void render_game();
    void main_menu();
    void main_choice(char choice);
    

    int get_current_level();
};


#endif