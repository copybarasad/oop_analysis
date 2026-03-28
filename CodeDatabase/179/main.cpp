#include <iostream>
#include <memory>
#include <string>
#include "GameController.h"
#include "GameView.h"
#include "CommandReader.h"
#include "GameRenderer.h"    // Включаем GameRenderer.h, а не ConsoleRenderer
#include "Logger.h"
#include "GameEventNotifier.h"

void setupLogger(int argc, char* argv[]) {
    bool logToFile = false;
    bool logToConsole = true;
    std::string logFile = "game.log";
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "--log-file" && i + 1 < argc) {
            logFile = argv[++i];
            logToFile = true;
        } else if (arg == "--no-console-log") {
            logToConsole = false;
        } else if (arg == "--help") {
            std::cout << "Usage: " << argv[0] << " [options]\n";
            std::cout << "Options:\n";
            std::cout << "  --log-file <filename>    Log to specified file\n";
            std::cout << "  --no-console-log         Disable console logging\n";
            std::cout << "  --help                   Show this help\n";
            exit(0);
        }
    }
    
    auto& logger = Logger::getInstance();
    logger.setLogToConsole(logToConsole);
    
    if (logToFile) {
        logger.setOutputFile(logFile);
    }
}

int main(int argc, char* argv[]) {
    try {
        std::cout << "========================================\n";
        std::cout << "          FANTASY BATTLE GAME\n";
        std::cout << "========================================\n";
        std::cout << "Lab Work #4 - Advanced Architecture\n";
        std::cout << "========================================\n\n";
        
        setupLogger(argc, argv);
        LOG_INFO("Starting Fantasy Battle Game");
        
        // Создаем CommandReader напрямую
        std::unique_ptr<ConsoleCommandReader> commandReader;
        try {
            commandReader = std::make_unique<ConsoleCommandReader>("controls.cfg");
            LOG_INFO("Command reader created with configuration file");
        } catch (const std::exception& e) {
            LOG_WARNING("Failed to load config file, using default: " + std::string(e.what()));
            commandReader = std::make_unique<ConsoleCommandReader>();
        }
        
        // Создание рендерера
        auto renderer = std::make_unique<ConsoleRenderer>();
        LOG_INFO("Console renderer initialized");
        
        // Создание контроллера с конкретным типом
        GameController<ConsoleCommandReader> controller(std::move(commandReader));
        GameView<ConsoleRenderer> view(std::move(renderer));
        
        // Связывание вью с игрой
        view.setGame(controller.getGame());
        
        LOG_INFO("Starting main game loop");
        
        // Запуск игры
        controller.start();
        
        LOG_INFO("Game finished normally");
        
        std::cout << "\n========================================\n";
        std::cout << "          Thanks for playing!\n";
        std::cout << " Game design and programming by @sekusidze\n";
        std::cout << "========================================\n";
        
    } catch (const std::exception& e) {
        LOG_ERROR("Fatal error: " + std::string(e.what()));
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
