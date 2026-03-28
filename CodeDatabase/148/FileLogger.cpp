#include "FileLogger.h"
#include <iostream>
#include <filesystem>

/**
 * @file FileLogger.cpp
 * @brief Реализация файлового логгера
 */

/**
 * @brief Конструктор
 */
FileLogger::FileLogger(const std::string& filename) 
    : filepath(filename), isOpen(false) {
    
    // Создаём папку logs если её нет
    std::filesystem::path logPath(filename);
    std::filesystem::path logDir = logPath.parent_path();
    
    if (!logDir.empty() && !std::filesystem::exists(logDir)) {
        std::filesystem::create_directories(logDir);
    }
    
    // Открываем файл
    logFile.open(filename, std::ios::out | std::ios::app);
    
    if (logFile.is_open()) {
        isOpen = true;
        logFile << "\n=== NEW GAME SESSION ===\n";
        logFile << "Started at: " << GameEvent(EventType::LEVEL_STARTED, "").getTimestampString() << "\n";
        logFile << "========================\n\n";
        logFile.flush();
    } else {
        std::cerr << "[FileLogger] Failed to open log file: " << filename << "\n";
    }
}

/**
 * @brief Деструктор
 */
FileLogger::~FileLogger() {
    close();
}

/**
 * @brief Записать событие
 */
void FileLogger::logEvent(const GameEvent& event) {
    if (!isOpen) return;
    
    logFile << "[" << event.getTimestampString() << "] "
            << "[" << event.getTypeString() << "] "
            << event.description << "\n";
    
    logFile.flush();  // Сразу записываем в файл
}

/**
 * @brief Закрыть файл
 */
void FileLogger::close() {
    if (isOpen) {
        logFile << "\n=== SESSION ENDED ===\n\n";
        logFile.close();
        isOpen = false;
    }
}


