#include "game.h"
#include "controller.h"
#include "visualizer.h"
#include "console.h"
#include "render.h"
#include "game_save.h"
#include <iostream>
#include <fstream>
#include <limits>

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    bool keep_app_running = true;

    while (keep_app_running) {
        std::cout << " _____ _   _ _____    ___   ___  ____     ____    _    __  __ _____ \n";
        std::cout << "|_   _| | | | ____|  / _ \\\\ / _ \\\\|  _ \\\\   / ___|  / \\\\  |  \\\\/  | ____|\n";
        std::cout << "  | | | |_| |  _|   | | | | | | | |_) | | |  _  / _ \\\\ | |\\\\/| |  _|  \n";
        std::cout << "  | | |  _  | |___  | |_| | |_| |  __/  | |_| |/ ___ \\\\| |  | | |___ \n";
        std::cout << "  |_| |_| |_|_____|  \\\\___/ \\\\___/|_|      \\\\____/_/   \\\\_\\\\_|  |_|_____|\n";
        
        std::string savefile = "game_save.txt";
        GameSave save_system;
        
        std::ifstream filecheck(savefile);
        bool save_exists = filecheck.good();
        filecheck.close();
        
        game* game_instance = nullptr;
        bool game_started = false;
        
        while (!game_started) {
            if (game_instance) delete game_instance;
            game_instance = new game();
            
            if (save_exists) {
                std::cout << "Found save file!\n";
                std::cout << "1 - New Game\n";
                std::cout << "2 - Load Game\n";
                std::cout << "Choose: ";

                std::string input_str;
                std::cin >> input_str;

                if (input_str == "1") {
                    std::cout << "Starting new game...\n";
                    game_instance->initializeGame();
                    game_instance->print_info();
                    game_started = true;
                } else if (input_str == "2") {
                    try {
                        std::cout << "Loading game...\n";
                        save_system.loadGame(game_instance, savefile);
                        game_started = true;
                    } catch (const SaveGameException& e) {
                        std::cerr << "Error: " << e.what() << "\n";
                    }
                } else {
                    std::cout << "Invalid input\n";
                }
            } else {
                std::cout << "No save file found. Starting new game...\n";
                game_instance->initializeGame();
                game_instance->print_info();
                game_started = true;
            }
        }

        clearInput();

        if (game_instance) {
            GameController<ConsoleInputReader> controller(*game_instance);
            GameVisualizer<ConsoleRenderer> visualizer;

            std::cout << "Game Loop Started.\n";

            while (!game_instance->isGameOver()) {
                visualizer.draw(*game_instance);
                controller.update();
            }

            delete game_instance;
            game_instance = nullptr;
        }
        
        std::cout << "\nGame Over.\n";
        std::cout << "1 - Start New Game\n";
        std::cout << "2 - Exit\n";
        std::cout << "Choose: ";
        
        std::string choice;
        std::cin >> choice;
        
        if (choice != "1") {
            keep_app_running = false;
            std::cout << "Exiting...\n";
        } else {
             std::system("clear");
        }
    }
    
    return 0;
}
