#ifndef GAME_H
#define GAME_H

#include "gamefield.h"
#include <string>

class Game {
private:
    Field field_;
    bool game_running_;
    int current_level_;

public:
    Game(int width, int height);
    
    void reset_game();
    bool is_running() const { return game_running_; }
    bool is_game_over() const;
    void quit() { game_running_ = false; }
    const Field& get_field() const { return field_; }
    
    bool move_player(int dx, int dy);
    void player_attack(int dx, int dy);
    void switch_weapon(bool to_melee);
    bool handle_spell_cast_input();
    bool buy_spell_interaction();
    
    void save_game(const std::string& filename);
    void load_game(const std::string& filename);
    
    void process_enemy_turns();
    void next_level();

private:
    void level_up_menu(Player* p);

};

#endif