#include "game.h"
#include "console_input_reader.h"
#include "console_renderer.h"
#include "game_controller.h"
#include "game_visualizer.h"
#include <iostream>
#include <memory>
#include <limits>
#include <string>

int main() {
    bool shouldContinue = true;  // Флаг продолжения работы программы

    while (shouldContinue) {
        try {
            Game game;
            GameController<ConsoleInputReader> controller(game);
            GameVisualizer<ConsoleRenderer> visualizer;

            // Загружаем настройки управления
            try {
                controller.loadKeyBindings("controls.ini");
                std::cout << "Controls loaded from controls.ini" << std::endl;
            } catch (...) {
                std::cout << "Using default controls" << std::endl;
            }

            // === ГЛАВНОЕ МЕНЮ ===
            bool inGame = false;

            while (!inGame) {
                visualizer.clearScreen();
                std::cout << "=== DUNGEON ADVENTURE ===\n"
                          << "1. New Game\n"
                          << "2. Load Game\n"
                          << "3. Controls\n"
                          << "4. Quit\n"
                          << "=========================\n"
                          << "Select option (1-4): ";

                std::string menuChoice;
                std::getline(std::cin, menuChoice);

                if (menuChoice.empty()) continue;

                char choice = menuChoice[0];

                if (choice == '1') {
                    // Новая игра
                    game.initializeGame();
                    inGame = true;
                } 
                else if (choice == '2') {
                    // Загрузить игру
                    try {
                        game.loadGame("savegame.dat");
                        std::cout << "Game loaded successfully! Press Enter to start..." << std::endl;
                        std::cin.get();
                        inGame = true;
                    } catch (const std::exception& e) {
                        std::cout << "Failed to load game: " << e.what() << std::endl;
                        std::cout << "Press Enter to continue..." << std::endl;
                        std::cin.get();
                    }
                } 
                else if (choice == '3') {
                    // Показать управление
                    visualizer.renderControls();
                    std::cout << "\nPress Enter to continue..." << std::endl;
                    std::cin.get();
                } 
                else if (choice == '4') {
                    // Выход из программы
                    std::cout << "Goodbye!" << std::endl;
                    shouldContinue = false;  // Устанавливаем флаг выхода
                    break;  // Выходим из цикла меню
                }
                else {
                    std::cout << "Invalid choice! Press Enter to continue..." << std::endl;
                    std::cin.get();
                }
            }

            
            if (!shouldContinue) {
                break;
            }

            // === ОСНОВНОЙ ИГРОВОЙ ЦИКЛ ===
            while (game.isGameRunning()) {
                // Отрисовка
                visualizer.renderGame(game);

                // Обработка ввода
                bool shouldContinueGame = controller.processInput();
                if (!shouldContinueGame) break;

                // Обновление игры
                game.updateGame();

                // Проверка условий
                game.checkGameConditions();
            }

            // === ПОСЛЕ ИГРЫ ===
            if (!game.isGameRunning()) {
                visualizer.renderGameOver();

                // Предложение рестарта
                std::cout << "\nPress 'R' to restart or any other key to return to menu: ";
                std::string choice;
                std::getline(std::cin, choice);

                if (choice.empty() || (choice[0] != 'r' && choice[0] != 'R')) {
                    // Возврат в меню - продолжаем внешний цикл
                    continue;
                }

            }

        } catch (const std::exception& e) {
            std::cerr << "Fatal error: " << e.what() << std::endl;
            std::cout << "Press Enter to return to menu..." << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        }
    }

    return 0;
}
