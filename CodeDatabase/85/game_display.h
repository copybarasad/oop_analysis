#ifndef GAME_DISPLAY_H
#define GAME_DISPLAY_H

#include "game_controller.h"

class Game_display {
private:
    const Game_controller& game_controller;

public:
    Game_display(const Game_controller& controller);
    void display_game_field() const;
    void display_help() const;
    void display_game_over() const;
};

#endif