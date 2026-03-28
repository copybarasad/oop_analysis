#ifndef GAMERUNNER_H
#define GAMERUNNER_H

#include <string>
#include <iostream>
#include "Game.h"
#include "managers.h"
#include "input_handler.h"
#include "rendering.h"

using ConsoleGameController = GameController<ConsoleInput>;
using ConsoleGameView = GameView<ConsoleRenderer>;

class GameRunner {
private:
    int current_level = 1;
    bool is_loaded_game = false;

    bool handle_startup_menu();
    
    void run_level(Game& game, ConsoleGameController& controller, ConsoleGameView& view);

public:
    int run();
};

#endif