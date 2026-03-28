#include "event_logger.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

std::string formatTimestamp(const std::chrono::system_clock::time_point& tp) {
    auto time = std::chrono::system_clock::to_time_t(tp);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void ConsoleEventLogger::onEvent(const std::shared_ptr<GameEvent>& event) {
    if (!event) return;
    
    std::cout << "[" << formatTimestamp(event->timestamp) << "] "
              << "[" << GameEvent::typeToString(event->type) << "] "
              << event->description << "\n";
}

FileEventLogger::FileEventLogger(const std::string& logFilename)
    : filename(logFilename) {
    ensureFileOpen();
}

FileEventLogger::~FileEventLogger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void FileEventLogger::ensureFileOpen() const {
    if (!logFile.is_open()) {
        logFile.open(filename, std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "Warning: Could not open log file: " << filename << "\n";
        }
    }
}

void FileEventLogger::onEvent(const std::shared_ptr<GameEvent>& event) {
    if (!event) return;
    
    ensureFileOpen();
    
    if (logFile.is_open()) {
        logFile << "[" << formatTimestamp(event->timestamp) << "] "
                << "[" << GameEvent::typeToString(event->type) << "] "
                << event->description << "\n";
        logFile.flush();
    }
}