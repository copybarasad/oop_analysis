#include "controllers/Game.h"
#include "controllers/GameManager.h"
#include "controllers/RenderController.h"
#include "controllers/SettingsController.h"
#include "controllers/input/InputController.h"
#include "views/GeneralView.h"
#include "models/events/Logger.h"
#include "models/events/EventNotifier.h"
#include <memory>
#include <string>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    // Инициализация системы логирования
    std::vector<LogOutputType> logTypes = {LogOutputType::CONSOLE};
    std::string logFile = "game.log";
    
    // Парсинг аргументов командной строки
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--log-file" || arg == "-f") {
            if (i + 1 < argc) {
                logFile = argv[++i];
            }
            logTypes = {LogOutputType::FILE};
        } else if (arg == "--log-console" || arg == "-c") {
            logTypes = {LogOutputType::CONSOLE};
        } else if (arg == "--log-both" || arg == "-b") {
            if (i + 1 < argc) {
                logFile = argv[++i];
            }
            logTypes = {LogOutputType::FILE, LogOutputType::CONSOLE};
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "Использование: " << argv[0] << " [опции]\n";
            std::cout << "Опции:\n";
            std::cout << "  --log-file, -f <файл>    Логировать в файл\n";
            std::cout << "  --log-console, -c        Логировать в консоль (по умолчанию)\n";
            std::cout << "  --log-both, -b [файл]   Логировать в файл и консоль\n";
            std::cout << "  --help, -h               Показать эту справку\n";
            return 0;
        }
    }
    
    try {
        Logger& logger = Logger::getInstance();
        logger.initialize(logTypes, logFile);
        EventNotifier::getInstance().subscribe(&logger);
    } catch (const std::exception& e) {
        std::cerr << "Ошибка инициализации логирования: " << e.what() << std::endl;
        return 1;
    }
    
    // Создаем view и render controller
    RenderController<GeneralView> renderController;
    
    // Создаем объект игры (класс игры)
    Game game;
    
    // Создаем контроллер настроек
    SettingsController settingsController("settings.json");
    GameManager<InputController> gameManager(&game, &settingsController);
    
    // Запускаем игру
    gameManager.startGame();
    
    return 0;
}
