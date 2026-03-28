// main.cpp
#include "Game.h"
#include "GameLoopController.h"
#include "SimpleConsoleInputProcessor.h"
#include "Logger.h"
#include "GameEventManager.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>

void parseCommandLineArgs(int argc, char* argv[],
    std::string& logFile,
    LogLevel& logLevel,
    bool& consoleOutput) {
    logFile = "game.log";
    logLevel = LogLevel::INFO;
    consoleOutput = true;  // По умолчанию показываем в консоли

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "--log-file" && i + 1 < argc) {
            logFile = argv[++i];
        }
        else if (arg == "--log-level" && i + 1 < argc) {
            std::string level = argv[++i];
            std::transform(level.begin(), level.end(), level.begin(), ::toupper);

            if (level == "DEBUG") logLevel = LogLevel::DEBUG;
            else if (level == "INFO") logLevel = LogLevel::INFO;
            else if (level == "WARNING") logLevel = LogLevel::WARNING;
            else if (level == "ERROR") logLevel = LogLevel::ERROR;
            else if (level == "CRITICAL") logLevel = LogLevel::CRITICAL;
        }
        else if (arg == "--no-console-log") {
            consoleOutput = false;
        }
        else if (arg == "--help") {
            std::cout << "Usage: " << argv[0] << " [options]\n"
                << "Options:\n"
                << "  --log-file <filename>    Set log file name (default: game.log)\n"
                << "  --log-level <level>      Set log level: DEBUG, INFO, WARNING, ERROR, CRITICAL\n"
                << "  --no-console-log         Disable console output\n"
                << "  --help                   Show this help message\n";
            exit(0);
        }
    }
}

int main(int argc, char* argv[]) {
    std::string logFile;
    LogLevel logLevel;
    bool consoleOutput;

    parseCommandLineArgs(argc, argv, logFile, logLevel, consoleOutput);

    std::filesystem::path fullPath = std::filesystem::absolute(logFile);
    std::cout << "Game log will be saved to: " << fullPath.string() << std::endl;

    if (!Logger::getInstance().initialize(logFile, logLevel, consoleOutput)) {
        std::cerr << "Failed to initialize logger. Continuing without logging." << std::endl;
    }

    LOG_INFO("=== GAME START ===");
    LOG_INFO("Command line arguments processed");
    LOG_INFO("Log file: " + logFile);
    LOG_INFO("Log level: " + std::to_string(static_cast<int>(logLevel)));

    try {
        Map map(20, 20);
        Game game(map);

        LOG_INFO("Game objects created successfully");

        GameLoopController<SimpleConsoleInputProcessor> controller(&game);

        controller.start();

        LOG_INFO("=== GAME END ===");
    }
    catch (const std::exception& e) {
        LOG_ERROR("Fatal exception: " + std::string(e.what()));
        std::cerr << "Fatal exception: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        LOG_ERROR("Unknown fatal exception occurred");
        std::cerr << "Unknown fatal exception" << std::endl;
        return 1;
    }

    std::cout << "\n=== PROGRAM ENDED ===" << std::endl;
    std::cout << "Log file: " << std::filesystem::absolute(logFile).string() << std::endl;

    return 0;
}