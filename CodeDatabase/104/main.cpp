#include "Game.h"
#include "GameSaveManager.h"
#include <iostream>

int main() {
    try {
        std::cout << "=== MAGIC BATTLE ===\n";
        std::cout << "1. New Game\n";
        std::cout << "2. Load Game\n";
        std::cout << "Choose option: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        std::unique_ptr<Game> game;

        if (choice == 2) {
            // Пытаемся загрузить игру при старте
            try {
                GameSaveManager saveManager;
                if (saveManager.saveExists("autosave.txt")) {
                    game = saveManager.loadGame("autosave.txt");
                    std::cout << "Game loaded successfully!\n";
                }
                else {
                    std::cout << "No save file found. Starting new game.\n";
                    game = std::make_unique<Game>();
                }
            }
            catch (const std::exception& e) {
                std::cout << "Load failed: " << e.what() << ". Starting new game.\n";
                game = std::make_unique<Game>();
            }
        }
        else {
            game = std::make_unique<Game>();
        }

        // Запускаем игровой цикл
        game->StartGame();
        game->ProcessGameLoop();

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}