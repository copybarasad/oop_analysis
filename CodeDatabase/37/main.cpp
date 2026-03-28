#include "Game.h"
#include "GameController.h"
#include "GameVisualizer.h"
#include "ConsoleInputHandler.h"
#include "ConsoleRenderer.h"
#include <iostream>

int main() {
    Game game;
    

    GameController<ConsoleInputHandler> controller(game);
    
    GameVisualizer<ConsoleRenderer> visualizer(game);
    
    std::cout << "=== LAB WORK 4 - ARCHITECTURE DEMO ===\n";
    std::cout << "Using templated controller and visualizer\n";
    std::cout << "Defeat all enemies (E) to win!\n";
    
    visualizer.showMessage("Game started!");

    while (game.isRunning()) {
        visualizer.render();
        
        controller.processInput();
    }

    visualizer.showMessage("Thanks for playing!");
    return 0;
}