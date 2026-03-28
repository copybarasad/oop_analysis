#include "GameLogger.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstring>

GameLogger::GameLogger(LogMode mode, const std::string& filename)
    : mode(mode), logFileName(filename), isFileOpen(false) {
    
    if (mode == LogMode::FILE || mode == LogMode::BOTH) {
        logFile.open(logFileName, std::ios::out | std::ios::app);
        isFileOpen = logFile.is_open();
        if (!isFileOpen) {
            std::cerr << "Warning: Could not open log file: " << logFileName << std::endl;
            std::cerr << "Falling back to console logging only." << std::endl;
            this->mode = LogMode::CONSOLE;
        }
    }
    
    // Подписываемся на события
    EventManager::getInstance().subscribe(this);
}

GameLogger::~GameLogger() {
    EventManager::getInstance().unsubscribe(this);
    closeLogFile();
}

void GameLogger::onEvent(const std::shared_ptr<GameEvent>& event) {
    if (!event) return;
    
    std::string message = event->toString();
    std::string fullMessage = "[" + event->getTimestamp() + "] " + message;
    
    if (mode == LogMode::CONSOLE || mode == LogMode::BOTH) {
        logToConsole(fullMessage);
    }
    
    if ((mode == LogMode::FILE || mode == LogMode::BOTH) && isFileOpen) {
        logToFile(fullMessage);
    }
}

void GameLogger::logToConsole(const std::string& message) {
    std::cout << "[LOG] " << message << std::endl;
}

void GameLogger::logToFile(const std::string& message) {
    if (isFileOpen && logFile.is_open()) {
        logFile << message << std::endl;
        logFile.flush(); // Гарантируем запись в файл
    }
}

void GameLogger::setMode(LogMode newMode) {
    mode = newMode;
    
    if ((mode == LogMode::FILE || mode == LogMode::BOTH) && !isFileOpen) {
        logFile.open(logFileName, std::ios::out | std::ios::app);
        isFileOpen = logFile.is_open();
        if (!isFileOpen) {
            std::cerr << "Warning: Could not open log file: " << logFileName << std::endl;
            mode = LogMode::CONSOLE;
        }
    }
}

void GameLogger::setLogFile(const std::string& filename) {
    closeLogFile();
    logFileName = filename;
    
    if (mode == LogMode::FILE || mode == LogMode::BOTH) {
        logFile.open(logFileName, std::ios::out | std::ios::app);
        isFileOpen = logFile.is_open();
        if (!isFileOpen) {
            std::cerr << "Warning: Could not open log file: " << logFileName << std::endl;
            mode = LogMode::CONSOLE;
        }
    }
}

void GameLogger::closeLogFile() {
    if (isFileOpen && logFile.is_open()) {
        logFile.close();
        isFileOpen = false;
    }
}

std::unique_ptr<GameLogger> GameLogger::createFromArgs(int argc, char* argv[]) {
    LogMode mode = LogMode::CONSOLE; // По умолчанию
    std::string filename = "game.log";
    
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--log" || arg == "-l") {
            if (i + 1 < argc) {
                std::string logType = argv[i + 1];
                std::transform(logType.begin(), logType.end(), logType.begin(), ::tolower);
                
                if (logType == "console" || logType == "con") {
                    mode = LogMode::CONSOLE;
                } else if (logType == "file") {
                    mode = LogMode::FILE;
                } else if (logType == "both") {
                    mode = LogMode::BOTH;
                }
                i++; // Пропускаем следующий аргумент
            }
        } else if (arg == "--log-file" || arg == "-f") {
            if (i + 1 < argc) {
                filename = argv[i + 1];
                i++; // Пропускаем следующий аргумент
            }
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: " << argv[0] << " [options]\n";
            std::cout << "Options:\n";
            std::cout << "  --log, -l <mode>     Logging mode: console, file, or both (default: console)\n";
            std::cout << "  --log-file, -f <file> Log file name (default: game.log)\n";
            std::cout << "  --help, -h           Show this help message\n";
            std::cout << "\nExamples:\n";
            std::cout << "  " << argv[0] << " --log console\n";
            std::cout << "  " << argv[0] << " --log file --log-file mygame.log\n";
            std::cout << "  " << argv[0] << " --log both\n";
        }
    }
    
    return std::make_unique<GameLogger>(mode, filename);
}

