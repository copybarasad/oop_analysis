#pragma once

#include "field.h"
#include "game_cycle.h"
#include "game.h"
#include "draw.h"
#include <string>

class Command{
private:
    Field& field;
    bool& running;
    Game& game;
    Renderer& renderer;
public:
    Command(Field& f, bool& r, Game& g, Renderer& rndr)
     : field(f), running(r), game(g), renderer(rndr) {}

    Field& get_field()  { return field; }
    Game&  get_game()   { return game; }
    bool&  get_running(){ return running; }

    void display_help() const;
    void move_command(const string& direction);
    void mode_switch();

    void attack(int x, int y);
    void use_spell(std::size_t idx, int x, int y);
    void save_game();
    void load_game();
};