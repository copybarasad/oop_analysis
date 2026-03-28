#include "game.h"
#include "exceptions.h"
#include "input_handler.h"   
#include "game_renderer.h"    
#include "game_controller.h" 
#include "game_visualizer.h"  
#include <iostream>
#include <string>
#include <limits>

int main() {
    setlocale(LC_ALL, "Russian");

    try {
        Game game; 

        GameController<ConsoleInputHandler> controller(game);
        GameVisualizer<ConsoleRenderer> visualizer(game);

        std::cout << "Добро пожаловать в игру!" << std::endl;

        while (true) {
            std::cout << "Хотите загрузить сохраненную игру? (y/n): ";
            char choice;
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (choice == 'y' || choice == 'Y') {
                std::cout << "Введите имя файла сохранения: ";
                std::string filename;
                std::getline(std::cin, filename);
                game.loadExistingGame(filename);
                break;
            }
            else if (choice == 'n' || choice == 'N') {
                std::cout << "Создана новая игра!" << std::endl;
                game.setupNewGame();
                break;
            }
            else {
                std::cout << "Некорректный ввод. Пожалуйста, введите 'y' или 'n'." << std::endl;
            }
        }

        while (game.isRunning()) {
            visualizer.render();
            visualizer.displayPlayerTurnInfo();

            Command command = controller.getPlayerCommand();

            bool turn_was_made = game.processPlayerCommand(command);

            if (!turn_was_made) {
                if (!game.isRunning()) {
                    break;
                }
                continue;
            }

            bool level_changed = game.updateGameState();

            if (!game.isRunning()) {
                break;
            }

            if (level_changed) {
                continue;
            }

            visualizer.render();

            game.handleEnemyTurn();

            game.updateGameState();
            if (!game.isRunning()) {
                break;
            }
        }

        if (game.getPlayer() && !game.getPlayer()->isAlive()) {
            std::cout << "\n*********************************" << std::endl;
            std::cout << "***         ВЫ ПРОИГРАЛИ!     ***" << std::endl;
            std::cout << "*********************************" << std::endl;
        }
        std::cout << "Спасибо за игру!" << std::endl;

    }
    catch (const GameException& e) {
        std::cerr << "Произошла критическая ошибка в игре: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Произошла непредвиденная системная ошибка: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Произошла неизвестная ошибка." << std::endl;
        return 1;
    }

    return 0;
}