#include "Game.h"
#include "GameSaveSystem.h"
#include "GameManager.h"
#include "InputHandler.h"
#include "GameVisualizer.h"
#include "ConsoleRenderer.h"
#include <iostream>
#include <vector>

void showMainMenu() {
    std::cout << "=================================" << std::endl;
    std::cout << "       DUNGEON ADVENTURE" << std::endl;
    std::cout << "=================================" << std::endl;
    std::cout << "1. New Game" << std::endl;
    std::cout << "2. Load Game" << std::endl;
    std::cout << "3. Exit" << std::endl;
    std::cout << "Choose option: ";
}

void runGame(Game& game) {
    GameManager<InputHandler> gameManager;
    GameVisualizer<ConsoleRenderer> gameVisualizer;
    
    gameManager.run(game, gameVisualizer);
}

void showLoadMenu() {
    bool loadMenuActive = true;
    
    while (loadMenuActive) {
        std::cout << "\n=================================" << std::endl;
        std::cout << "            LOAD GAME"                 << std::endl;
        std::cout << "================================="   << std::endl;
        
        std::vector<std::string> saves = GameSaveSystem::getSaveFiles();
        
        if (saves.empty()) {
            std::cout << "No saved games found." << std::endl;
            std::cout << "Start new game instead? (Y/N or B - Back): ";
            
            char choice;
            std::cin >> choice;
            
            if (choice == 'Y' || choice == 'y') {
                Game game;
                runGame(game);
                loadMenuActive = false;
            } else if (choice == 'B' || choice == 'b') {
                loadMenuActive = false;
            } else {
                continue;
            }
        } else {
            std::cout << "Available saves:" << std::endl;
            for (size_t i = 0; i < saves.size(); ++i) {
                std::cout << i + 1 << ". " << saves[i] << std::endl;
            }
            std::cout << "0. Back to Main Menu" << std::endl;
            
            std::cout << "Choose save file: ";
            int choice;
            std::cin >> choice;
            
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Invalid choice!" << std::endl;
                continue;
            }
            
            if (choice == 0) {
                loadMenuActive = false;
            } else if (choice < 1 || choice > saves.size()) {
                std::cout << "Invalid choice!" << std::endl;
            } else {
                Game game;
                if (GameSaveSystem::loadGame(game, saves[choice - 1])) {
                    runGame(game);
                }
                loadMenuActive = false; 
            }
        }
    }
}

int main() {
    try {
        bool shouldExit = false;
        
        while (!shouldExit) {
            showMainMenu();
            
            int choice;
            std::cin >> choice;
            
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Invalid input! Please enter 1, 2 or 3" << std::endl;
                continue;
            }
            
            switch (choice) {
                case 1: {
                    std::cout << "\nStarting new game..." << std::endl;
                    Game game;
                    runGame(game);
                    break;  
                }
                case 2: {
                    showLoadMenu();
                    break;
                }
                case 3: {
                    std::cout << "Thanks for playing! Goodbye!" << std::endl;
                    shouldExit = true;
                    break;
                }
                default: {
                    std::cout << "Invalid choice! Please enter 1, 2 or 3" << std::endl;
                    break;
                }
            }
        }
        
    } catch (const std::exception& e) {
        std::cout << "Critical error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}