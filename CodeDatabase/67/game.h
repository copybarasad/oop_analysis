#ifndef GAME_H
#define GAME_H

#include <vector>
#include <random>
#include "game_field.h"
#include "player.h"
#include "enemy.h"
#include "enemy_tower.h"
#include "entity.h"
#include "game_save.h"


class game{

friend class GameSave;

private:
    game_field* field_ptr;
    Player* player_ptr;
    GameSave save_system;
    std::vector<Enemy*> enemies;
    std::vector<EnemyTower*> towers;
    bool is_game_over;
    int next_enemy_id;
    int turn;
    int current_level;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dis1;
    std::uniform_int_distribution<> dis2;
    void showEndGameMenu();
    std::vector<std::string> message_log;

public:
    game();
    ~game();

    void add_log(const std::string& msg);
    const std::vector<std::string>& get_log() const;

    void quit() { is_game_over = true; }

    Player* get_player() const { return player_ptr;}

    int get_level() const { return current_level; }

    void saveGameToFile(const std::string& filename);


    void cleanup();
    void initializeGame();
    void loadGameDuringPlay(const std::string& filename);
    game_field* get_field() const;
    int get_turn() const;
    bool isGameOver() const;
    
    void PlayerTurn(int dx, int dy);
    void castSpell();
    void RewardSpell();
    void enemy_turn();
    void enemy_movement(Enemy* enemy_ptr, int enemy_x,int enemy_y, int player_x, int player_y);
    void enemy_deal_damage(Enemy* enemy_ptr);
    void tower_turn();
    void spawnEnemy();
    void spawnTower();
    void initializeGameEmpty();
    Enemy* get_Enemy_at(int x, int y) const;
    EnemyTower* get_Tower_at(int x, int y) const;
    void erase_dead_Enemies();
    bool allEnemiesAndTowersDestroyed() const;
    void checkGameOver();
    void checkGameWin();
    void next_turn();
    void print_info();
    //void game_run();
    void erase_destroyed_towers();
    void clearScreen();
    void spawnEnemyAt(int x, int y);
    void spawnTowerAt(int x, int y);
    void nextLevel();
    void levelUpMenu();
};

#endif
