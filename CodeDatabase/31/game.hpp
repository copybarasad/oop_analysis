#ifndef GAME_HPP
#define GAME_HPP

#include "entities/player.hpp"
#include "field/field.hpp"
#include "level.hpp"
#include "draw.hpp"
#include "input_reader.hpp"
#include "game_controller.hpp"
#include "game_visualizer.hpp"
#include "loger.hpp"

template<typename> class GameController;

class Game {
    Player *player;
    int width;
    int height;
    Field *field;
    Hand *hand;
    Cursor cursor;
    Draw *draw;
    Level *current_level;
    int level_number;
    Loger *logger;
    GameController<InputReader> *controller;
    GameVisualizer<Draw> *visualizer;

public:
    Game() : player(nullptr), field(nullptr), hand(nullptr), draw(nullptr), 
             current_level(nullptr), level_number(1), logger(nullptr),
             controller(nullptr), visualizer(nullptr) {}
    
    ~Game();
    
    void calculate_field_size();
    void restore_termios_impl();
    Point rand_pos();
    void init_game();
    void game_loop();
    void add_entity(Entity *entity);
    void delete_entity(Entity *entity);
    void next_level();
    void save_game();
    void save_level_checkpoint();
    void load_level_checkpoint();
    bool load_game();
    bool prompt_load_or_new();
    void set_cursor_pos();
    
    void player_move_up();
    void player_move_down();
    void player_move_left();
    void player_move_right();
    void player_attack();
    void player_switch_attack();
    void show_hand();
    void buy_spell();
    void use_spell();
    void exit_game();
    void load_game_cmd();
    void show_help();
    
    void make_steps();
    
    Player* get_player() const { return player; }
    Field* get_field() const { return field; }
    Hand* get_hand() const { return hand; }
    Level* get_current_level() const { return current_level; }
    Cursor& get_cursor() { return cursor; }
    Draw* get_draw() const { return draw; }
    GameVisualizer<Draw>* get_visualizer() const { return visualizer; }
    GameController<InputReader>* get_controller() const { return controller; }
    bool is_cursor_enabled() const { return cursor.is_enabled; }
    void set_cursor_enabled(bool enabled) { cursor.is_enabled = enabled; }
};

#endif