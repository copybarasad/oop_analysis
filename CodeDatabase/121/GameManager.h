#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Command.h"
#include "LevelController.h"
#include "ActionSystem.h"

template<typename TInputHandler>
class GameManager {
private:
    TInputHandler& inputHandler;
    ActionSystem& actionSystem;
    
public:
    explicit GameManager(TInputHandler& inputHandlerRef, ActionSystem& actionSys) 
        : inputHandler(inputHandlerRef), actionSystem(actionSys) {}
    
    Command getPlayerCommand() {
        return inputHandler.getCommand();
    }
    
    bool processCommand(Command command, LevelController& level) {
        return actionSystem.processCommand(command, level);
    }
};

#endif