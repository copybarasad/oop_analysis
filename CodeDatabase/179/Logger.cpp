#include "Logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>

Logger* Logger::instance = nullptr;

Logger::Logger() : logToConsole(true), logToFile(false) {}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

Logger& Logger::getInstance() {
    if (!instance) {
        instance = new Logger();
    }
    return *instance;
}

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    
    std::string logEntry = getCurrentTime() + " [" + getLevelString(level) + "] " + message;
    
    if (logToConsole) {
        // Разные цвета для разных уровней (на Unix-системах)
        #ifdef __unix__
            switch (level) {
                case LogLevel::INFO: std::cout << "\033[32m"; break; // Green
                case LogLevel::WARNING: std::cout << "\033[33m"; break; // Yellow
                case LogLevel::ERROR: std::cout << "\033[31m"; break; // Red
                case LogLevel::DEBUG_LEVEL: std::cout << "\033[36m"; break; // Cyan
                case LogLevel::GAME_EVENT: std::cout << "\033[35m"; break; // Magenta
            }
        #endif
        
        std::cout << logEntry << std::endl;
        
        #ifdef __unix__
            std::cout << "\033[0m"; // Reset color
        #endif
    }
    
    if (logToFile && logFile.is_open()) {
        logFile << logEntry << std::endl;
    }
}

void Logger::setOutputFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(logMutex);
    
    if (logFile.is_open()) {
        logFile.close();
    }
    
    logFile.open(filename, std::ios::app);
    if (logFile.is_open()) {
        logToFile = true;
        log(LogLevel::INFO, "Logging to file: " + filename);
    } else {
        log(LogLevel::ERROR, "Cannot open log file: " + filename);
        logToFile = false;
    }
}

void Logger::setLogToConsole(bool enable) {
    std::lock_guard<std::mutex> lock(logMutex);
    logToConsole = enable;
}

void Logger::setLogToFile(bool enable) {
    std::lock_guard<std::mutex> lock(logMutex);
    logToFile = enable;
}

std::string Logger::getLevelString(LogLevel level) const {
    switch (level) {
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::DEBUG_LEVEL: return "DEBUG";  // Здесь возвращаем "DEBUG" для вывода
        case LogLevel::GAME_EVENT: return "GAME_EVENT";
        default: return "UNKNOWN";
    }
}

std::string Logger::getCurrentTime() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    
    std::tm tm;
    #ifdef _WIN32
        localtime_s(&tm, &time);
    #else
        localtime_r(&time, &tm);
    #endif
    
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    
    // Добавляем миллисекунды
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    
    return oss.str();
}
