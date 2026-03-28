#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "Command.h"
// #include "Game.h"  // Убираем циклическое включение

class Game;  // Forward declaration

class InputHandler {
public:
    virtual ~InputHandler() = default;
    virtual void handle(Game& game, Command cmd) = 0;
};

#endif // INPUT_HANDLER_H