#include "CommandLineParser.h"
#include <iostream>
#include <algorithm>

/**
 * @file CommandLineParser.cpp
 * @brief Реализация парсера аргументов командной строки
 */

/**
 * @brief Распарсить аргументы
 */
CommandLineArgs CommandLineParser::parse(int argc, char* argv[]) {
    CommandLineArgs args;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        // Справка
        if (arg == "--help" || arg == "-h") {
            args.showHelp = true;
            continue;
        }
        
        // Парсим аргументы с =
        if (arg.find('=') != std::string::npos) {
            auto [key, value] = splitArgument(arg);
            
            if (key == "--log-to") {
                if (value == "console") {
                    args.logMode = LogMode::CONSOLE;
                } else if (value == "file") {
                    args.logMode = LogMode::FILE;
                } else if (value == "both") {
                    args.logMode = LogMode::BOTH;
                } else if (value == "none") {
                    args.logMode = LogMode::NONE;
                } else {
                    std::cerr << "Unknown log mode: " << value << "\n";
                    std::cerr << "Valid options: console, file, both, none\n";
                }
            } else if (key == "--log-file") {
                args.logFile = value;
            } else if (key == "--config") {
                args.configFile = value;
            } else {
                std::cerr << "Unknown argument: " << key << "\n";
            }
        } else {
            std::cerr << "Invalid argument format: " << arg << "\n";
        }
    }
    
    return args;
}

/**
 * @brief Показать справку
 */
void CommandLineParser::printHelp() {
    std::cout << "\n=== DUNGEON CRAWLER - COMMAND LINE ARGUMENTS ===\n\n";
    
    std::cout << "Usage: ./game [OPTIONS]\n\n";
    
    std::cout << "Options:\n";
    std::cout << "  --log-to=MODE          Set logging mode\n";
    std::cout << "                         MODE can be: console, file, both, none\n";
    std::cout << "                         Default: console\n\n";
    
    std::cout << "  --log-file=PATH        Set log file path\n";
    std::cout << "                         Default: logs/game.log\n\n";
    
    std::cout << "  --config=PATH          Set keybindings config file\n";
    std::cout << "                         Default: config/keybindings.txt\n\n";
    
    std::cout << "  --help, -h             Show this help message\n\n";
    
    std::cout << "Examples:\n";
    std::cout << "  ./game                                    # Default settings\n";
    std::cout << "  ./game --log-to=file                      # Log to file only\n";
    std::cout << "  ./game --log-to=both --log-file=my.log    # Log to console and my.log\n";
    std::cout << "  ./game --log-to=none                      # Disable logging\n";
    std::cout << "  ./game --config=custom_keys.txt           # Custom keybindings\n";
    
    std::cout << "\n================================================\n\n";
}

/**
 * @brief Разбить аргумент на ключ и значение
 */
std::pair<std::string, std::string> CommandLineParser::splitArgument(const std::string& arg) {
    size_t pos = arg.find('=');
    if (pos == std::string::npos) {
        return {arg, ""};
    }
    
    std::string key = arg.substr(0, pos);
    std::string value = arg.substr(pos + 1);
    
    return {key, value};
}


