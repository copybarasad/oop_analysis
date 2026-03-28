#include "game.h"
#include "game_controller.h"
#include "input_reader.h"
#include "logger.h"
#include "console_renderer.h"
#include "game_exceptions.h"
#include <iostream>
#include <exception>
#include <limits>
#include <memory>
#include <string>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

using namespace rpg;

struct LogConfig {
    bool log_to_file = false;
    bool log_to_console = false;
    std::string log_filename = "game.log";
};

LogConfig ParseCommandLineArgs(int argc, char* argv[]) {
    LogConfig config;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "--log-file") {
            config.log_to_file = true;
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                config.log_filename = argv[++i];
            }
        } else if (arg == "--log-console") {
            config.log_to_console = true;
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "Использование: rpg_game [опции]\n";
            std::cout << "Опции:\n";
            std::cout << "  --log-file [имя_файла]  Логировать события в файл (по умолчанию: game.log)\n";
            std::cout << "  --log-console           Логировать события в консоль\n";
            std::cout << "  --help, -h              Показать эту справку\n";
            std::cout << "\nПримеры:\n";
            std::cout << "  rpg_game --log-file                    # Логировать только в game.log\n";
            std::cout << "  rpg_game --log-file my.log             # Логировать только в my.log\n";
            std::cout << "  rpg_game --log-console                 # Логировать только в консоль\n";
            std::cout << "  rpg_game --log-file --log-console      # Логировать и в файл, и в консоль\n";
            std::exit(0);
        }
    }
    
    return config;
}

int main(int argc, char* argv[]) {
    try {
#ifdef _WIN32
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
#endif

        LogConfig log_config = ParseCommandLineArgs(argc, argv);

        Game game;
        
        std::unique_ptr<FileLogger> file_logger;
        std::unique_ptr<ConsoleLogger> console_logger;
        
        if (log_config.log_to_file) {
            file_logger = std::make_unique<FileLogger>(log_config.log_filename);
            game.GetEventManager().Subscribe(file_logger.get());
            std::cout << "Логирование в файл включено: " << log_config.log_filename << "\n";
        }
        
        if (log_config.log_to_console) {
            console_logger = std::make_unique<ConsoleLogger>();
            game.GetEventManager().Subscribe(console_logger.get());
            std::cout << "Логирование в консоль включено\n";
        }
        
        std::string config_file = "controls.cfg";
        GameController<ConsoleInputReader> controller(game, config_file);
        
        #ifdef _WIN32
        system("cls");
        #else
        std::cout << "\033[2J\033[1;1H";
        #endif
        
        bool game_running = true;
        while (game_running) {
            std::cout << "\n=== RPG ИГРА ===\n";
            std::cout << "1. Новая игра\n";
            std::cout << "2. Загрузить игру\n";
            std::cout << "3. Выход\n";
            std::cout << "Выберите опцию: ";
            
            int choice;
            std::cin >> choice;
            
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Ошибка ввода! Введите число от 1 до 3.\n";
                continue;
            }
            
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (choice == 3) {
                std::cout << "До свидания!\n";
                game_running = false;
                continue;
            }

            try {
                if (choice == 1) {
                    game.StartNewGame();
                    controller.RunGameLoop();
                } else if (choice == 2) {
                    game.LoadGameFromFile();
                    controller.RunGameLoop();
                } else {
                    std::cout << "Неверный выбор! Введите число от 1 до 3.\n";
                    continue;
                }
            } catch (const GameException& e) {
                std::cout << "\nОшибка: " << e.what() << "\n";
                std::cout << "Нажмите Enter для продолжения...\n";
                std::cin.get();
                continue;
            } catch (const std::exception& e) {
                std::cout << "\nНеожиданная ошибка: " << e.what() << "\n";
                std::cout << "Нажмите Enter для продолжения...\n";
                std::cin.get();
                continue;
            }
            
            std::cout << "\nХотите выйти из игры полностью? (Y/n): ";
            char restart;
            std::cin >> restart;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            if (restart != 'n' && restart != 'N') {
                game_running = false;
            }
        }

        if (file_logger) {
            game.GetEventManager().Unsubscribe(file_logger.get());
        }
        if (console_logger) {
            game.GetEventManager().Unsubscribe(console_logger.get());
        }

        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "\nКРИТИЧЕСКАЯ ОШИБКА: " << e.what() << "\n";
        std::cerr << "Программа будет завершена.\n";
        std::cerr << "Нажмите Enter для выхода...\n";
        std::cin.get();
        return 1;
    } catch (...) {
        std::cerr << "\nКРИТИЧЕСКАЯ ОШИБКА: Неизвестное исключение\n";
        std::cerr << "Программа будет завершена.\n";
        std::cerr << "Нажмите Enter для выхода...\n";
        std::cin.get();
        return 1;
    }
}
