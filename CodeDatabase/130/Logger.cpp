#include "Logger.h"
#include <iostream>

void ConsoleLogger::log(const GameEvent& event) {
    std::cout << "[LOG] " << event.message << "\n";
}

FileLogger::FileLogger(const std::string& path) {
    out_.open(path, std::ios::app);
}

void FileLogger::log(const GameEvent& event) {
    if (out_.is_open()) {
        out_ << event.message << "\n";
    }
}

std::shared_ptr<ILogger> LoggerFactory::createConsole() {
    return std::make_shared<ConsoleLogger>();
}

std::shared_ptr<ILogger> LoggerFactory::createFile(const std::string& path) {
    return std::make_shared<FileLogger>(path);
}


