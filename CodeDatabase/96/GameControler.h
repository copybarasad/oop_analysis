#pragma once
#include <string>
#include <memory>
#include <string.h>
#include "Command.h"

class Game;

template <typename InputHandler>
class GameController {
private:
    InputHandler input;

public:
    bool process(Game& game){
        int a1, a2;
        Command cmd = input.getCommand(a1, a2);

        game.ProcessCommand(cmd, a1, a2);
        return true;
    }
};
