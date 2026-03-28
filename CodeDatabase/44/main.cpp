#include <cstdlib>
#include <ctime>
#include <clocale>
#include <iostream>

#include "Game.h"
#include "EventDispatcher.h"
#include "EventLogger.h"

void PrintUsage(const char* program_name) {
    std::cout << "Использование: " << program_name << " [опции]\n"
              << "Опции:\n"
              << "  --log-console, -lc     Логирование в консоль\n"
              << "  --log-file, -lf [file] Логирование в файл (по умолчанию game.log)\n"
              << "  --no-log               Без логирования (по умолчанию)\n"
              << "  --help, -h             Показать справку\n";
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    srand(static_cast<unsigned int>(time(0)));

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            PrintUsage(argv[0]);
            return 0;
        }
    }

    std::string log_filename;
    auto logger_type = LoggerFactory::ParseArgs(argc, argv, log_filename);
    auto logger = LoggerFactory::Create(logger_type, log_filename);
    
    if (logger) {
        EventDispatcher::Instance().SetLogger(std::move(logger));
        std::cout << "Логирование включено";
        if (logger_type == LoggerFactory::LoggerType::FILE) {
            std::cout << " (файл: " << log_filename << ")";
        } else {
            std::cout << " (консоль)";
        }
        std::cout << "\n";
    }

    Game game(10, 10, 20, 3, 1);
    game.Start();
    return 0;
}
