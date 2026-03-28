#include "Game.h"
#include "GameLoop.h"
#include "ConsoleInputReader.h"
#include "ConsoleRenderer.h"
#include "KeyBindingConfig.h"
#include "GameEventSystem.h"
#include "ConsoleLogger.h"
#include "FileLogger.h"
#include "CommandLineParser.h"
#include "MenuSystem.h"
#include <iostream>
#include <memory>

int main(int argc, char* argv[]) {
    // ========== 1. Парсинг аргументов командной строки ==========
    CommandLineArgs args = CommandLineParser::parse(argc, argv);
    
    if (args.showHelp) {
        CommandLineParser::printHelp();
        return 0;
    }
    
    // ========== 2. Инициализация логирования ==========
    std::unique_ptr<ConsoleLogger> consoleLogger;
    std::unique_ptr<FileLogger> fileLogger;
    
    if (args.logMode == LogMode::CONSOLE || args.logMode == LogMode::BOTH) {
        consoleLogger = std::make_unique<ConsoleLogger>(true, true);
        GameEventSystem::getInstance().subscribe(consoleLogger.get());
    }
    
    if (args.logMode == LogMode::FILE || args.logMode == LogMode::BOTH) {
        fileLogger = std::make_unique<FileLogger>(args.logFile);
        if (fileLogger->isFileOpen()) {
            GameEventSystem::getInstance().subscribe(fileLogger.get());
        } else {
            std::cerr << "Warning: Failed to open log file. File logging disabled.\n";
        }
    }
    
    // ========== 3. Загрузка конфигурации клавиш ==========
    KeyBindingConfig keyConfig;
    try {
        keyConfig.loadFromFile(args.configFile);
    } catch (const std::exception& e) {
        std::cerr << "Warning: Failed to load config: " << e.what() << "\n";
        std::cerr << "Using default key bindings.\n";
    }
    
    // ========== 4. Главное меню и игровой цикл ==========
    bool exitProgram = false;
    
    while (!exitProgram) {
        // Показать главное меню
        auto mainChoice = MenuSystem::showMainMenu();
        
        if (mainChoice == MenuSystem::MainMenuChoice::EXIT) {
            std::cout << "\nThank you for playing!\n";
            break;
        }
        
        // Создать игру
        Game game;
        
        // Загрузка сохранения (если выбрано)
        if (mainChoice == MenuSystem::MainMenuChoice::LOAD_GAME) {
            std::string saveName = MenuSystem::showLoadMenu();
            
            if (!saveName.empty()) {
                try {
                    game.loadGame(saveName);
                    std::cout << "Game loaded successfully!\n";
                } catch (const std::exception& e) {
                    std::cerr << "Failed to load game: " << e.what() << "\n";
                    std::cout << "Starting new game instead...\n";
                }
            }
        }
        
        // ========== ЗАПУСК ИГРОВОГО ЦИКЛА ==========
        GameLoop<ConsoleInputReader, ConsoleRenderer> gameLoop(&game);
        gameLoop.run();
    }
    
    // ========== 5. Завершение работы ==========
    if (fileLogger) {
        fileLogger->close();
    }
    
    std::cout << "\nGoodbye!\n";
    return 0;
}


