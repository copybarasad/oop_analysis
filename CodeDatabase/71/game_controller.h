#pragma once

#include "game.h"
#include "command.h"

template <typename InputPolicy>
class GameController {
public:
    GameController(Game& game) : game_(game) {}

    Command getPlayerCommand() {
        return input_handler_.getCommand();
    }

private:
    Game& game_;
    InputPolicy input_handler_; 
};