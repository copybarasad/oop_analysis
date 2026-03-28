#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "GameEngine.h"
#include "Command.h"
#include <memory>

class MovementCommand;
class CastSpellCommand;

template<typename InputHandler>
class GameController {
private:
    InputHandler inputHandler;
    GameEngine* gameEngine;

public:
    GameController(GameEngine* engine);
    
    std::unique_ptr<Command> getNextCommand();
    
    bool processGameCommand(std::unique_ptr<Command>& command);
};

#include "GameController.tpp"

#endif