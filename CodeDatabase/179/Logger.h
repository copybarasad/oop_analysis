#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <memory>
#include <mutex>

enum class LogLevel {
    INFO,
    WARNING,
    ERROR,
    DEBUG_LEVEL,    // Изменено с DEBUG на DEBUG_LEVEL
    GAME_EVENT
};

class Logger {
private:
    static Logger* instance;
    std::ofstream logFile;
    std::mutex logMutex;
    bool logToConsole;
    bool logToFile;
    
    Logger();
    
public:
    ~Logger();
    
    // Delete copy constructor and assignment operator
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    static Logger& getInstance();
    
    void log(LogLevel level, const std::string& message);
    void setOutputFile(const std::string& filename);
    void setLogToConsole(bool enable);
    void setLogToFile(bool enable);
    
private:
    std::string getLevelString(LogLevel level) const;
    std::string getCurrentTime() const;
};

// Обновленные макросы с новым именем
#define LOG_INFO(msg) Logger::getInstance().log(LogLevel::INFO, msg)
#define LOG_WARNING(msg) Logger::getInstance().log(LogLevel::WARNING, msg)
#define LOG_ERROR(msg) Logger::getInstance().log(LogLevel::ERROR, msg)
#define LOG_DEBUG(msg) Logger::getInstance().log(LogLevel::DEBUG_LEVEL, msg)  // Изменено
#define LOG_GAME_EVENT(msg) Logger::getInstance().log(LogLevel::GAME_EVENT, msg)

#endif
