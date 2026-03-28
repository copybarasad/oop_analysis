#ifndef GAME_GAME_H_
#define GAME_GAME_H_

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <stdexcept>

#include "save_exception.h"
#include "load_exception.h"
#include "player.h"
#include "enemy.h"
#include "game_field.h"
#include "hand.h"
#include "action_type.h"
#include "action.h"

class Game {
public:
    Game();

    void run();
    void execute(const PlayerAction& action);

    const GameField& get_field() const { return field_; }
    const Player& get_player() const { return player_; }
    const std::vector<Enemy>& get_enemies() const { return enemies_; }

    Enemy* get_enemy_at(int row, int col);
    void remove_dead_enemies();
    void spawn_enemy(int health, int damage, int row, int col);

    bool save(const std::string& filename) const noexcept;
    bool load(const std::string& filename) noexcept;

    void start_level();
    bool check_victory_condition() const;
    bool check_defeat_condition() const;

private:
    void start();
    void enemies_turn();
    void ask_restart_or_exit(bool& running);
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);

    GameField field_;
    Player player_;
    Hand hand_;
    std::vector<Enemy> enemies_;

    bool game_over_;
    bool level_complete_;
};


#endif // GAME_GAME_H_
