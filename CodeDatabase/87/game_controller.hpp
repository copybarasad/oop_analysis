#ifndef GAME_CONTROLLER_HPP
#define GAME_CONTROLLER_HPP
#include <iostream>
#include <vector>
#include <memory>
#include "player.hpp"
#include "enemy.hpp"
#include "gamefield.hpp"
#include "enemy_tower.hpp"
#include "position.hpp"
#include "event_logger.hpp"
#include "game_event.hpp"

class Game_controller{
    private:
        std::unique_ptr<Player> player;
        std::vector<std::unique_ptr<Enemy>> enemies;
        Gamefield game_field;
        bool game_over;
    
        void move_enemies();

        struct Trap{
            Position pos;
            int damage;
        };
        std::vector<Trap> traps;
        std::vector<Enemy_tower> towers;
        int current_level;
        int initial_enemy_count;
        bool level_completed;

        Event_logger logger;

    public:
        Game_controller(int field_height, int field_width, int enemy_count);
    
        void process_player_move(const Position& new_position);
        void process_player_combat_style_switch(Combat_style new_style);
        void process_player_ranged_attack();
        void process_enemy_turn();
    
        const Player& get_player() const{return *player;};
        Player& get_player(){return *player;}
        const Gamefield& get_game_field() const{return game_field;};
        const std::vector<std::unique_ptr<Enemy>>& get_enemies() const{return enemies;};
        bool is_game_finished() const{return game_over;};

        void spawn_enemies(int enemy_count, int enemy_damage);

        void process_player_spell_cast(int spell_index, Position target_pos);
        void process_player_buy_spell();

        void place_trap(Position& pos, int damage);
        void check_traps_on_enemies();
        const std::vector<Trap>& get_traps() const {return traps;}
        const std::vector<Enemy_tower>& get_towers() const{return towers;}

        void spawn_towers(int enemy_count);
        bool is_position_free_for_enemy(const Position& pos) const;
        void attack_player_with_tower(Enemy_tower& tower);

        void clear_all_entities();
        void add_enemy(std::unique_ptr<Enemy> enemy);
        void add_tower(const Enemy_tower& tower);
        void add_trap(const Game_controller::Trap& trap);

        void reset_player(int health, int close_damage, int ranged_damage, const Position& pos);

        Game_controller(int field_height, int field_width, std::vector<std::vector<Cell_type>>&& field_grid);
        void load_from_file(const std::string& filename);

        void proceed_to_next_level();
        int get_current_level() const{return current_level;}
        void set_current_level(int level){current_level = level;}

        int get_initial_enemy_count() const{return initial_enemy_count;}
        void set_initial_enemy_count(int count){initial_enemy_count = count;}
        bool is_level_completed() const{return level_completed;}

        void set_logging_enabled(Event_logger::Log_mode mode, const std::string& filename);
        void note_event(const Game_event& event);
        void on_game_over(bool player_won);
        void on_level_complete(int level);
};

#endif