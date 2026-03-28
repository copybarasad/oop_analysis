#ifndef GAME_GAMEMANAGER_H
#define GAME_GAMEMANAGER_H

#include "Game.h"
#include "input/IInputHandler.h"
#include "rendering/IRenderer.h"
#include "observers/ConsoleLogger.h"
#include <memory>
#include <iostream>
#include <limits>

namespace Game {

template <typename InputHandler, typename Renderer>
class GameManager {
public:
    // Конструктор теперь принимает KeyConfig для передачи в InputHandler
    GameManager(Game& game, Renderer& renderer, bool pauseForLogs, const KeyConfig& config) 
        : gameInstance(game), renderer(renderer), inputHandler(game, config), pauseForLogs(pauseForLogs) {}

    LevelResult run() {
        gameInstance.notify({EventType::LevelStarted, gameInstance.getLevelNumber()});
        
        while (!gameInstance.hasEnded()) {
            renderer.render(gameInstance);
            
            std::unique_ptr<ICommand> command = inputHandler.getCommand();
            
            gameInstance.runTurn(*command);

            if (pauseForLogs && !gameInstance.hasEnded()) {
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
            }
        }
        
        renderer.render(gameInstance);
        std::cout << "\nLevel finished. Press Enter to return to the main menu.";
        std::cin.get();
        
        return gameInstance.getResult();
    }

private:
    Game& gameInstance;
    Renderer& renderer;
    InputHandler inputHandler;
    bool pauseForLogs;
};

}

#endif