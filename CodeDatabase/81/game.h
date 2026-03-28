#ifndef GAME_H
#define GAME_H

#include <string>
#include <random>
#include <iostream>
#include "constants.h"
#include "Player.h" 
#include "Enemy.h"
#include "Tower.h"
#include "Field.h"
#include "Hand.h"

template <typename InputProvider> class GameController;
template <typename RendererT> class GameView;
class ConsoleRenderer;

class Game {
    std::mt19937 gen;
    int max_enemies = NUMBER_OF_ENEMIES;
    int max_towers = NUMBER_OF_TOWERS;
    Player player;
    int level;
    Field field;
    std::vector<Enemy> enemies; 
    std::vector<Tower> towers;
    int counter_of_enemies = NUMBER_OF_ENEMIES;
    int counter_of_towers = NUMBER_OF_TOWERS;
    int counter_of_holes = NUMBER_OF_HOLES;
    Hand hand;
    int new_lvl = 0;

public:
    Game();
    Game(int lvl);
    Player& getPlayer() { return player; }
    Field& getField() { return field; }
    Hand& getHand() { return hand; }
    int& getCounterEnemies() { return counter_of_enemies; }
    int& getCounterTowers() { return counter_of_towers; }
    int getLevel() { return level; }
    void start();
    void place_object_at_random(ObjectType obj, std::uniform_int_distribution<int>& distrib_x, std::uniform_int_distribution<int>& distrib_y, Creature *creature);
    void initialize();
    int player_turn();
    int enemy_turn();
    int tower_turn();
    int run();
    bool save_game(const std::string& filename = "save.dat");
    bool load_game(const std::string& filename = "save.dat");
};

#endif