#include <iostream>
#include <string>
#include <fstream>
#include <limits>

#include "Game.hpp"
#include "logger.hpp"
#include "input_handler.hpp"
#include "view.hpp"
#include "game_controller.hpp"


void clearInputMain() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main(int argc, char* argv[]) {
    try {
        Game game; 
        
        std::cout << "=== MAIN MENU ===\n";
        std::cout << "1. New Game\n";
        std::cout << "2. Load Game\n";
        std::cout << "> ";
        
        int mode;
        if (!(std::cin >> mode)) {
            std::cout << "Invalid input. Starting New Game by default.\n";
            std::cin.clear();
            clearInputMain();
            mode = 1; 
        }

        if (mode == 2) {
            try { 
                game.loadGame("savegame.txt"); 
                std::cout << "Game loaded successfully.\n";
            }
            catch (const std::exception& e) { 
                std::cout << "Load failed: " << e.what() << "\nStarting new game...\n"; 
                std::cout << "Press Enter to continue...";
                clearInputMain(); 
                std::cin.get();
                
                game.initLevel(true); 
            }
        } else {
            game.initLevel(true);
        }

        std::ostream* logStream = &std::cout;
        std::ofstream fileStream;

        if (argc > 1 && std::string(argv[1]) == "file") {
            fileStream.open("gamelog.txt");
            if (fileStream.is_open()) {
                logStream = &fileStream;
            } else {
                std::cerr << "Failed to open log file. Using console." << std::endl;
            }
        }

        GameLogger logger(*logStream);
        game.attachLogger(&logger); 

        GameManager<ConsoleInputHandler> manager(&game, "controls.cfg");
        GameVisualizer<ConsoleRenderer> visualizer;

        while (game.isRunning()) {
            visualizer.draw(game, manager.getInputHandler());

            if (game.getBoard() && game.getBoard()->getPlayer() && !game.getBoard()->getPlayer()->isAlive()) {
                std::cout << "\n=== GAME OVER ===\n";
                std::cout << "Press Enter to exit.";
                if (std::cin.rdbuf()->in_avail() > 0) clearInputMain();
                std::cin.get(); 
                break;
            }

            manager.processInput();
        }

    } catch (const std::exception& e) {
        std::cerr << "\nCRITICAL ERROR: " << e.what() << std::endl;
        std::cout << "Press Enter to exit.";
        std::cin.clear();
        clearInputMain();
        std::cin.get();
        return 1;
    }
    
    return 0;
}