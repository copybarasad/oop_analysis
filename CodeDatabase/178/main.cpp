#include <iostream>
#include <string>
#include <cstring>
#include "Game.h"
#include "Exceptions.h"
#include "Logger.h"

void showUsage(const char* programName) {
    std::cout << "\n=== RPG Game ===" << std::endl;
    std::cout << "Usage: " << programName << " [options]" << std::endl;
    std::cout << "\nOptions:" << std::endl;
    std::cout << "  --config <file>       Load controls configuration from file" << std::endl;
    std::cout << "  --log-console         Log to console only" << std::endl;
    std::cout << "  --log-file            Log to file only (game.log)" << std::endl;
    std::cout << "  --log-both            Log to both console and file (default)" << std::endl;
    std::cout << "  --help                Show this help message" << std::endl;
    std::cout << "\nExamples:" << std::endl;
    std::cout << "  " << programName << "                      # Run with default settings" << std::endl;
    std::cout << "  " << programName << " --config mycontrols.txt  # Use custom controls" << std::endl;
    std::cout << "  " << programName << " --log-file            # Log to file only" << std::endl;
}

LogOutput parseLogOutput(const std::string& arg) {
    if (arg == "--log-console") return LogOutput::CONSOLE;
    if (arg == "--log-file") return LogOutput::FILE;
    if (arg == "--log-both") return LogOutput::BOTH;
    return LogOutput::BOTH;
}

int main(int argc, char* argv[]) {
    try {
        std::string configFile = "controls.config";
        LogOutput logOutput = LogOutput::BOTH;
        std::string logFilename = "game.log";
        
        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];
            
            if (arg == "--config" && i + 1 < argc) {
                configFile = argv[++i];
                std::cout << "Using config file: " << configFile << std::endl;
            } 
            else if (arg == "--log-console" || arg == "--log-file" || arg == "--log-both") {
                logOutput = parseLogOutput(arg);
                std::cout << "Logging to: " << arg.substr(2) << std::endl;
            }
            else if (arg == "--help") {
                showUsage(argv[0]);
                return 0;
            }
            else {
                std::cerr << "Unknown option: " << arg << std::endl;
                showUsage(argv[0]);
                return 1;
            }
        }
        
        Logger::initialize(logOutput, logFilename);
        Logger::getInstance().logInfo("Game started");
        Logger::getInstance().logInfo("Command line arguments processed");
        if (!configFile.empty()) {
            Logger::getInstance().logInfo("Using config file: " + configFile);
        }
        
        Game game;
        game.run();
        
        Logger::getInstance().logInfo("Game ended");
        Logger::destroy();
        
    } 
    catch (const GameException& e) {
        Logger::getInstance().logError(std::string("Game error: ") + e.what());
        std::cerr << "Game error: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        Logger::getInstance().logError(std::string("Unexpected error: ") + e.what());
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "\nThanks for playing!" << std::endl;
    return 0;
}