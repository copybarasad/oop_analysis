#include "Logger.h"
#include "GameEvent.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>

Logger::Logger() : outputTypes({LogOutputType::CONSOLE}), logFilePath("game.log") {
}

Logger::~Logger() {
    close();
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::initialize(const std::vector<LogOutputType>& types, const std::string& filePath) {
    outputTypes = types;
    logFilePath = filePath;
    
    if (hasOutputType(LogOutputType::FILE)) {
        initializeFile();
    }
}

bool Logger::hasOutputType(LogOutputType type) const {
    return std::find(outputTypes.begin(), outputTypes.end(), type) != outputTypes.end();
}

void Logger::initializeFile() {
    if (logFile && logFile->is_open()) {
        logFile->close();
    }
    
    logFile = std::make_unique<std::ofstream>(logFilePath, std::ios::app);
    if (!logFile->is_open()) {
        throw std::runtime_error("Не удалось открыть файл для логирования: " + logFilePath);
    }
}

void Logger::writeToFile(const std::string& message) {
    if (logFile && logFile->is_open()) {
        *logFile << message << std::endl;
        logFile->flush();
    }
}

void Logger::writeToConsole(const std::string& message) {
    std::cout << message << std::endl;
}

void Logger::onEvent(const GameEvent& event) {
    // Игнорируем события рендеринга и вывода сообщений - они не должны попадать в логи
    EventType type = event.getType();
    if (type == EventType::FIELD_RENDER_REQUEST || type == EventType::MESSAGE_PRINT) {
        return;
    }
    
    std::string logMessage = event.toString();
    
    if (hasOutputType(LogOutputType::FILE)) {
        writeToFile(logMessage);
    }
    
    if (hasOutputType(LogOutputType::CONSOLE)) {
        writeToConsole(logMessage);
    }
}

void Logger::log(const std::string& message) {
    if (hasOutputType(LogOutputType::FILE)) {
        writeToFile(message);
    }
    
    if (hasOutputType(LogOutputType::CONSOLE)) {
        writeToConsole(message);
    }
}

void Logger::close() {
    if (logFile && logFile->is_open()) {
        logFile->close();
    }
}

