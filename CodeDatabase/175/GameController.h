#pragma once
#include "InputHandler.h"
#include "Game.h"

template<typename InputProvider>
class GameController {
private:
    InputProvider inputProvider;
    Game& game;
    
public:
    GameController(Game& gameRef, const std::string& configFile = "")
        : game(gameRef), inputProvider(configFile) {}
    
    void processInput() {
        auto command = inputProvider.getNextCommand();
        if (command) {
            command->execute(game);
        }
    }
};