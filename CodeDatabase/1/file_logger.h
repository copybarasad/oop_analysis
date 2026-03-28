#pragma once
#include "event_system.h"
#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>

// Базовый интерфейс логгера
class Logger : public EventListener {
public:
    virtual ~Logger() = default;
    virtual void shutdown() = 0;
    virtual bool isActive() const = 0;
};

// Логгер в файл
class FileLogger : public Logger {
private:
    std::ofstream log_file_;
    std::string filename_;
    bool active_;
    
    std::string getCurrentTimestamp() const {
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%H:%M:%S");
        return oss.str();
    }
    
public:
    explicit FileLogger(const std::string& filename);
    ~FileLogger();
    
    // Запрещаем копирование
    FileLogger(const FileLogger&) = delete;
    FileLogger& operator=(const FileLogger&) = delete;
    
    void onEvent(const GameEvent& event) override;
    void shutdown() override;
    bool isActive() const override { return active_; }
};

// Логгер в консоль
class ConsoleLogger : public Logger {
private:
    bool active_;
    
    std::string getColorForEvent(const std::string& eventType) const {
        if (eventType == "DAMAGE" || eventType == "DEATH") return "\033[31m"; // Красный
        if (eventType == "SPELL") return "\033[33m";  // Желтый
        if (eventType == "MOVE") return "\033[32m";   // Зеленый
        return "\033[0m";  // Стандартный
    }
    
    std::string getCurrentTimestamp() const {
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%H:%M:%S");
        return oss.str();
    }
    
public:
    ConsoleLogger();
    ~ConsoleLogger();
    
    // Запрещаем копирование
    ConsoleLogger(const ConsoleLogger&) = delete;
    ConsoleLogger& operator=(const ConsoleLogger&) = delete;
    
    void onEvent(const GameEvent& event) override;
    void shutdown() override;
    bool isActive() const override { return active_; }
};