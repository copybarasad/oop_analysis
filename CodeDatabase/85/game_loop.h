#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include "game_controller.h"
#include "game_display.h"

class Game_loop {
private:
    Game_controller& game;
    Game_display& display;

public:
    Game_loop(Game_controller& g, Game_display& d);
    void run();
};

#endif