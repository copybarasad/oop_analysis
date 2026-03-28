#include <iostream>
#include "Game.h"
#include <string>

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [--logger TYPE] [--logfile FILE]" << std::endl;
    std::cout << "  --logger TYPE    : Type of logger (console, file, both). Default: console" << std::endl;
    std::cout << "  --logfile FILE   : Log file name (used with file or both). Default: game.log" << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "  " << programName << " --logger console" << std::endl;
    std::cout << "  " << programName << " --logger file --logfile mygame.log" << std::endl;
    std::cout << "  " << programName << " --logger both --logfile game.log" << std::endl;
}

int main(int argc, char* argv[]) {
    std::string loggerType = "console";
    std::string logFile = "game.log";
    
    // Парсим аргументы командной строки
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--logger" && i + 1 < argc) {
            loggerType = argv[++i];
            if (loggerType != "console" && loggerType != "file" && loggerType != "both") {
                std::cerr << "Invalid logger type: " << loggerType << std::endl;
                printUsage(argv[0]);
                return 1;
            }
        } else if (arg == "--logfile" && i + 1 < argc) {
            logFile = argv[++i];
        } else if (arg == "--help" || arg == "-h") {
            printUsage(argv[0]);
            return 0;
        } else {
            std::cerr << "Unknown argument: " << arg << std::endl;
            printUsage(argv[0]);
            return 1;
        }
    }
    
    Game game(11, 11, loggerType, logFile);
    std::cout << "Game created! Logger: " << loggerType;
    if (loggerType == "file" || loggerType == "both") {
        std::cout << ", Log file: " << logFile;
    }
    std::cout << std::endl;
    game.run();
    return 0;
}