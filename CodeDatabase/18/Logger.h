#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

enum class EventType {
    Info,
    Combat,
    Movement,
    GameFlow,
    Error
};

class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(EventType type, const std::string& message) = 0;
};

class ConsoleLogger : public ILogger {
public:
    void log(EventType type, const std::string& message) override {
        std::string prefix = "[INFO]";
        if (type == EventType::Error) prefix = "[ERROR]";
        else if (type == EventType::Combat) prefix = "[COMBAT]";
        else if (type == EventType::GameFlow) prefix = "[GAME]";

        std::cout << prefix << " " << message << std::endl;
    }
};

class FileLogger : public ILogger {
private:
    std::ofstream ofs;
public:
    explicit FileLogger(const std::string& filename) {
        ofs.open(filename, std::ios::app);
    }
    ~FileLogger() {
        if (ofs.is_open()) ofs.close();
    }
    void log(EventType type, const std::string& message) override {
        if (ofs.is_open()) {
            ofs << static_cast<int>(type) << ": " << message << std::endl;
        }
    }
};

class EventManager {
private:
    std::vector<std::shared_ptr<ILogger>> loggers;
    static EventManager* instance;
    EventManager() = default;

public:
    static EventManager& getInstance() {
        static EventManager inst;
        return inst;
    }

    void addLogger(std::shared_ptr<ILogger> logger) {
        loggers.push_back(logger);
    }

    void notify(EventType type, const std::string& message) {
        for (auto& logger : loggers) {
            logger->log(type, message);
        }
    }
};