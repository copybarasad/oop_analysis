#pragma once
#include "InputProcessor.h"
#include "Command.h"
#include "MoveCommand.h"           
#include "SwitchCombatModeCommand.h"  
#include "CastSpellCommand.h"      
#include "PauseCommand.h"   
#include <memory>
#include <typeinfo>
#include <iostream>

class Game;

template<typename InputProcessorType>
class GameController {
private:
    std::unique_ptr<InputProcessorType> inputProcessor;
    Game* game; 

    void executeCommand(Command& command) {
        if (auto moveCmd = dynamic_cast<MoveCommand*>(&command)) {
            game->movePlayer(moveCmd->getDX(), moveCmd->getDY());
        }
        else if (dynamic_cast<SwitchCombatModeCommand*>(&command)) {
            game->switchCombatMode();
        }
        else if (dynamic_cast<CastSpellCommand*>(&command)) {
            game->castSpell();
        }
        else if (dynamic_cast<PauseCommand*>(&command)) {
            game->pauseGame();
        }
    }

public:
    GameController(Game* gameInstance) : inputProcessor(std::make_unique<InputProcessorType>()), game(gameInstance) { }

    void processNextCommand() {
        std::unique_ptr<Command> command = inputProcessor->processInput();
        if (command) {
            executeCommand(*command);
        }
    }
};