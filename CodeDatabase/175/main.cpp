#include <iostream>
#include <windows.h>
#include "Game.h"
#include "GameController.h"
#include "GameVisualizer.h"
#include "InputHandler.h"
#include "Renderer.h"
#include "Logger.h"
#include "EventSystem.h"
#include <memory>
#include <string>

void printUsage() {
    std::cout << "Использование: game [опции]" << std::endl;
    std::cout << "Опции:" << std::endl;
    std::cout << "  --console-log     Логирование только в консоль" << std::endl;
    std::cout << "  --file-log        Логирование только в файл" << std::endl;
    std::cout << "  --no-log          Без логирования" << std::endl;
    std::cout << "  --log-file=FILE   Указать файл для логирования" << std::endl;
    std::cout << "  --help            Показать эту справку" << std::endl;
}

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    LogOutput logType = LogOutput::BOTH;
    std::string logFile = "game.log";
    std::string controlConfig = "controls.cfg";
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "--console-log") {
            logType = LogOutput::CONSOLE;
        } else if (arg == "--file-log") {
            logType = LogOutput::FILE;
        } else if (arg == "--no-log") {
            logType = static_cast<LogOutput>(-1);
        } else if (arg == "--help" || arg == "-h") {
            printUsage();
            return 0;
        } else if (arg.find("--log-file=") == 0) {
            logFile = arg.substr(11);
        }
    }
    
    try {
        auto& eventSystem = EventSystem::getInstance();
        
        std::shared_ptr<Logger> logger;
        if (logType != static_cast<LogOutput>(-1)) {
            logger = std::make_shared<Logger>(logType, logFile);
            eventSystem.addObserver(logger);
            
            GameEvent initEvent;
            initEvent.type = EventType::GAME_LOADED;
            initEvent.actor = "System";
            initEvent.details = "Игра запущена";
            eventSystem.notify(initEvent);
        }
        
        Game game;
        GameController<KeyboardInputHandler> controller(game, controlConfig);
        GameVisualizer<ConsoleRenderer> visualizer(game);
        
        visualizer.clear();
        std::cout << "=== ПОШАГОВАЯ СТРАТЕГИЯ ===" << std::endl;
        std::cout << "1. Новая игра" << std::endl;
        std::cout << "2. Загрузить игру" << std::endl;
        std::cout << "3. Выход" << std::endl;
        std::cout << "Выберите вариант: ";
        
        int choice;
        std::cin >> choice;
        
        switch(choice) {
            case 1:
                game.startNewGame();
                break;
            case 2:
                if (!game.loadGame("save.txt")) {
                    std::cout << "Не удалось загрузить игру. Начинается новая игра." << std::endl;
                    game.startNewGame();
                }
                break;
            case 3:
                return 0;
            default:
                std::cout << "Неверный выбор. Начинается новая игра." << std::endl;
                game.startNewGame();
        }
        
        while (game.getStatus() == GameStatus::RUNNING) {
            visualizer.update();
            controller.processInput();
        }
        
        visualizer.clear();
        visualizer.render();
        
        if (game.getStatus() == GameStatus::PLAYER_WON) {
            std::cout << "\n=== ПОБЕДА! ===" << std::endl;
        } else if (game.getStatus() == GameStatus::PLAYER_LOST) {
            std::cout << "\n=== ПОРАЖЕНИЕ ===" << std::endl;
        }
        
        std::cout << "\nИгра завершена. Нажмите Enter для выхода...";
        std::cin.ignore();
        std::cin.get();
        
    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}