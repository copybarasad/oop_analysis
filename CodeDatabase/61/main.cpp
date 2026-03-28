#include "Game.h"
#include "GameException.h"
#include "GameController.h"
#include "CommandReader.h"
#include "GameLogger.h"
#include <iostream>
#include <stdexcept>
#include <limits>
#include <memory>

int main(int argc, char* argv[]) {
    try {
        // Инициализируем систему логирования из параметров командной строки
        std::unique_ptr<GameLogger> logger = GameLogger::createFromArgs(argc, argv);
        
        // Предлагаем загрузить игру или начать новую
        std::cout << "=== Game Menu ===" << std::endl;
        std::cout << "1 - New game" << std::endl;
        std::cout << "2 - Load game" << std::endl;
        std::cout << "Your choice: ";
        
        int choice = 0;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Starting new game.\n";
            choice = 1;
        }
        
        Game* game = nullptr;
        
        if (choice == 2) {
            // Загрузка игры
            std::string filename;
            std::cout << "Enter load filename (default: savegame.txt): ";
            std::cin.ignore();
            std::getline(std::cin, filename);
            if (filename.empty()) {
                filename = "savegame.txt";
            }
            
            // Создаем временную игру для загрузки
            game = new Game(15, 15);
            if (game->loadGame(filename)) {
                std::cout << "Game loaded successfully!" << std::endl;
            } else {
                std::cout << "Failed to load game. Starting new game instead.\n" << std::endl;
                delete game;
                choice = 1; // Переключаемся на новую игру
            }
        }
        
        if (choice == 1) {
            // Новая игра
            int width = 0;
            int height = 0;
            while (true) {
                std::cout << "Enter field width (10-25): ";
                if (!(std::cin >> width)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid input. Please enter an integer.\n";
                    continue;
                }
                std::cout << "Enter field height (10-25): ";
                if (!(std::cin >> height)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid input. Please enter an integer.\n";
                    continue;
                }
                if (width < 10 || width > 25 || height < 10 || height > 25) {
                    std::cout << "Field size must be between 10 and 25. Try again.\n";
                    continue;
                }
                break;
            }
            
            if (game) {
                delete game;
            }
            game = new Game(width, height);
        }
        
        // Run the game using GameController (which loads control configuration)
        GameController<CommandReader> controller(game);
        controller.run();
        
        delete game;
        
    } catch (const GameException& e) {
        std::cerr << "Game Error: " << e.what() << std::endl;
        std::cerr << "Location: " << e.getLocation() << std::endl;
        // Программа не завершается, пользователь может попробовать снова
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        // Программа не завершается, пользователь может попробовать снова
        return 0;
    } catch (...) {
        std::cerr << "Unknown error occurred." << std::endl;
        // Программа не завершается, пользователь может попробовать снова
        return 0;
    }
    
    return 0;
}

