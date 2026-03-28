#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include <memory>
#include <vector>
#include <string>
#include "player.h"
#include "enemy.h"
#include "gamefield.h"
#include "save_system.h"
#include "level_manager.h"

class Game_controller {
private:
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    Gamefield game_field;
    bool game_over;
    SaveSystem save_system;
    LevelManager level_manager;
    int base_width;
    int base_height;
    int base_enemy_count;
    
    int current_width;
    int current_height; 
    int current_enemy_count;

    void spawn_enemies(int enemy_count);
    void move_enemies();
    bool is_position_free_for_enemy(const Position& pos) const;
    bool has_obstacle_between(const Position& start, const Position& end) const;
    
    bool has_accessible_neighbors(const Position& pos) const;
    Position find_position_with_escape(int max_attempts) const;
    
    void initialize_new_level();
    void remove_half_spells();

public:
    Game_controller(int field_height, int field_width, int enemy_count);
    
    void load_player_state(int health, int max_health, int damage, const Position& position, 
                          int score, Combat_style style);
    void load_enemy_state(int health, int max_health, int damage, const Position& position);
    void clear_enemies();
    void load_gamefield_state(int width, int height, const std::vector<std::string>& map_data);
    
    void set_level_parameters(int width, int height, int enemy_count);
    int get_current_width() const { return current_width; }
    int get_current_height() const { return current_height; }
    int get_current_enemy_count() const { return current_enemy_count; }
    
    void process_player_move(const Position& new_position);
    void process_player_combat_style_switch(Combat_style new_style);
    void process_player_ranged_attack();
    void process_enemy_turn();
    
    void process_player_spell_cast(int spell_index, const Position& target);
    void process_player_get_spell();
    
    void advance_to_next_level();
    bool can_advance_level() const { return enemies.empty() && player->is_alive(); }
    int get_current_level() const { return level_manager.get_current_level(); }
    void set_level(int level) { level_manager.set_level(level); }
    
    void save_game();
    bool load_game();
    bool can_load_game() const;
    
    const Player& get_player() const;
    Player& get_player();
    const Gamefield& get_game_field() const;
    Gamefield& get_game_field();
    const std::vector<std::unique_ptr<Enemy>>& get_enemies() const;
    std::vector<std::unique_ptr<Enemy>>& get_enemies();
    bool is_game_finished() const { return game_over || !player->is_alive(); }
    bool is_game_over() const;
};

#endif