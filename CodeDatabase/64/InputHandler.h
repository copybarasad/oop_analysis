#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "Command.h"

class Game;

class InputHandler {
public:
    Command getCommand(const Game& game);
};

#endif