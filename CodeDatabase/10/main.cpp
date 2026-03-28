#include <iostream>
#include <string>
#include "game.h"
#include "console_input_provider.h"
#include "game_controller.h"
#include "game_visualization.h"
#include "console_renderer.h"
#include "command_handler.h"

int main() {
    CommandHandler::loadConfig("game_config.txt");
    
    Game game;
    ConsoleInputProvider inputProvider;
    
    GameVisualization<ConsoleRenderer> visualization;
    
    GameController<ConsoleInputProvider> controller(inputProvider);
    
    bool programRunning = true;
    
    std::cout << "ДОБРО ПОЖАЛОВАТЬ В ИГРУ!" << std::endl;
    
    while (programRunning) {
        try {
            std::cout << "\nГЛАВНОЕ МЕНЮ:" << std::endl;
            std::cout << "1. НОВАЯ ИГРА" << std::endl;
            std::cout << "2. ЗАГРУЗИТЬ ИГРУ" << std::endl;
            std::cout << "3. ВЫХОД" << std::endl;
            std::cout << "Выберите опцию: ";
            
            std::string choice;
            std::getline(std::cin, choice);
            
            if (choice == "1" || choice == "НОВАЯ" || choice == "NEW") {
                game.startNewGame();
            } else if (choice == "2" || choice == "ЗАГРУЗИТЬ" || choice == "LOAD") {
                if (!game.loadGame()) {
                    std::cout << "Не удалось загрузить игру. Начинаем новую игру" << std::endl;
                    game.startNewGame();
                }
            } else if (choice == "3" || choice == "ВЫХОД" || choice == "QUIT") {
                programRunning = false;
                continue;
            } else {
                std::cout << "Неверный выбор. Попробуйте снова." << std::endl;
                continue;
            }
            
            visualization.update(game);
            
            while (game.isRunning()) {
                game.updatePlayerEffects();
                if (controller.processCommand(game)) {
                    game.processEnemiesTurn();
                    game.processTowersTurn();
                    game.checkLevelCompletion();
                    game.checkGameOver();
                }
                visualization.update(game);
            }
            
        } catch (const std::exception& e) {
            visualization.showMessage("Ошибка: " + std::string(e.what()));
        }
    }
    
    std::cout << "Спасибо за игру! До свидания!" << std::endl;
    return 0;
}