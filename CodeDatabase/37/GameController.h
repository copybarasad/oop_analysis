#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <memory>
#include "InputHandler.h"
#include "Game.h"
#include <iostream>

template<typename InputHandlerType>
class GameController {
private:
    std::unique_ptr<InputHandlerType> inputHandler;
    Game& game;
    
public:
    GameController(Game& gameRef) 
        : inputHandler(std::make_unique<InputHandlerType>()), game(gameRef) {}
    
    void processInput() {
        if (inputHandler->hasInput()) {
            auto command = inputHandler->getNextCommand();
            if (command) {
                command->execute(game);
            }
            // If command is nullptr (invalid input), just continue
        }
    }
    
    void setInputHandler(std::unique_ptr<InputHandlerType> handler) {
        inputHandler = std::move(handler);
    }
};

#endif