#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <cstdlib>

#include "include/GameCycle.h"
#include "include/Manager.h"
#include "include/GameException.h"
#include "../entities/include/Player.h"
#include "../entities/include/Enemy.h"
#include "../entities/include/EnemyBuilding.h"
#include "../entities/include/EnemyTower.h"
#include "../entities/include/Ally.h"
#include "../items/include/Hand.h"
#include "../items/include/Spell.h"

#include "include/GameController.h"
#include "include/GameVisualizer.h"
#include "include/ConsoleInputReader.h"
#include "include/ConsoleRenderer.h"

int main() {
    GameCycle game;
    
    bool inMenu = true;
    while (inMenu) {
        int choice = ConsoleRenderer::showMainMenu();
        if (choice == 1) {
            int r, c;
            ConsoleRenderer::getFieldSize(r, c);
            game.setDimensions(r, c);
            game.startGame();
            inMenu = false;
        } else if (choice == 2) {
            std::string filename = ConsoleRenderer::getLoadFilename();
            try {
                game.loadGame(filename);
                std::cout << "Game loaded successfully!\n";
                std::cout << "Press Enter to continue...";
                std::cin.ignore(); std::cin.get();
                inMenu = false;
            } catch (const std::exception& e) {
                std::cout << "Failed to load game: " << e.what() << "\n";
                std::cout << "Press Enter to return to menu...";
                std::cin.ignore(); std::cin.get();
            }
        } else if (choice == 3) {
            return 0;
        }
    }

    bool restart = true;
    while (restart) {
        GameController<ConsoleInputReader> controller(game);
        GameVisualizer<ConsoleRenderer> visualizer;
        
        controller.run(visualizer);

        restart = false;
        if (game.isLose()) {
            std::cout << "\n=== GAME OVER ===\n";
            std::cout << "Restart? (y/n): ";
            char c; std::cin >> c;
            if (c == 'y') {
                game.startGame();
                restart = true;
            }
        }
    }

    return 0;
}
