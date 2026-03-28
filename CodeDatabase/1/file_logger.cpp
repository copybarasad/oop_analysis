#include "file_logger.h"
#include <iostream>

FileLogger::FileLogger(const std::string& filename) 
    : filename_(filename), active_(true) {
    
    log_file_.open(filename_, std::ios::app);
    if (!log_file_.is_open()) {
        active_ = false;
        std::cerr << "Не удалось открыть файл лога: " << filename_ << std::endl;
        return;
    }
    
    log_file_ << "=== НАЧАЛО ЛОГА ===\n";
    log_file_.flush();
}

FileLogger::~FileLogger() {
    shutdown();
}

void FileLogger::onEvent(const GameEvent& event) {
    if (!active_ || !log_file_.is_open()) return;
    
    std::string timestamp = getCurrentTimestamp();
    log_file_ << "[" << timestamp << "] " << event.toString() << "\n";
    log_file_.flush();
}

void FileLogger::shutdown() {
    if (active_ && log_file_.is_open()) {
        log_file_ << "=== КОНЕЦ ЛОГА ===\n\n";
        log_file_.close();
        active_ = false;
    }
}

ConsoleLogger::ConsoleLogger() : active_(true) {}

ConsoleLogger::~ConsoleLogger() {
    shutdown();
}

void ConsoleLogger::onEvent(const GameEvent& event) {
    if (!active_) return;
    std::string event_type = event.getType(); 
    std::string color = getColorForEvent(event.getType());
    std::string timestamp = getCurrentTimestamp();
    std::string reset = "\033[0m";
    if (event_type == "INPUT" || 
        event_type == "COMMAND" || 
        event_type == "VALIDATION" ||
        event_type == "UI_EVENT" ||
        event_type == "CONFIG") {
        return; 
    }
    std::cout << color 
              << "[" << timestamp << "] " 
              << event.toString() 
              << reset << std::endl;
}

void ConsoleLogger::shutdown() {
    active_ = false;
}