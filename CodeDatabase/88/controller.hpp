#ifndef GAMECONTROLLER_HPP
#define GAMECONTROLLER_HPP

#include "inputhandler.hpp"
#include "consoleinput.hpp"
#include "command.hpp"
#include "../managers/gamemanager.hpp"
#include <memory>
#include <iostream>
#include <string>
#include <functional>

enum class CommandResult {
    Success,
    UnknownCommand,
    GameEnd,
    SaveGame
};

template<typename InputHandlerType = ConsoleInputHandler>
class GameController {
private:
    InputHandlerType inputHandler;
    Game* game;
    std::function<void()> onGameStateChanged; 
    
public:
    GameController(Game* gameInstance, const std::string& configFile = "");
    
    void setOnGameStateChanged(std::function<void()> callback);
    std::unique_ptr<Command> getPlayerCommand();
    CommandResult processCommand(Command* command);
    InputHandlerType& getInputHandler() {return inputHandler;}
    
    void runGameLoop();
};

#endif