#include "Game.h"
#include "InputHandler.h"
#include "GameRenderer.h"
#include "GameController.h"
#include "GameVisualizer.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    srand(time(0));
    
    Game game;
    InputHandler inputHandler("config.txt");
    ConsoleRenderer renderer;
    
    GameController<InputHandler> controller(game, inputHandler);
    GameVisualizer<ConsoleRenderer> visualizer(game, renderer);
    
    std::cout << "=== RPG GAME ===" << std::endl;
    std::cout << "=================" << std::endl;
    
    // Главное меню
    if (!controller.runMainMenu()) {
        std::cout << "Failed to start game." << std::endl;
        return 1;
    }
    
    std::cout << "\nGame started! Good luck!" << std::endl;

    while (game.isRunning()) {
        visualizer.visualize();
        controller.processInput();
        
        if (!controller.isInMenu() && game.isRunning()) {
            game.updateGameState();
        }
    }
    
    std::cout << "\nThanks for playing!" << std::endl;
    return 0;
}