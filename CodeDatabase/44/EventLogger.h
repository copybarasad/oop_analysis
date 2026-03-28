#pragma once
#include "GameEvent.h"
#include <string>
#include <fstream>
#include <memory>

class IEventLogger {
public:
    virtual ~IEventLogger() = default;
    virtual void Log(const GameEvent& event) = 0;
};

class ConsoleLogger : public IEventLogger {
public:
    void Log(const GameEvent& event) override;
private:
    std::string FormatEvent(const GameEvent& event) const;
};

class FileLogger : public IEventLogger {
public:
    explicit FileLogger(const std::string& filename);
    ~FileLogger();
    void Log(const GameEvent& event) override;
private:
    std::string FormatEvent(const GameEvent& event) const;
    std::ofstream file_;
};

class LoggerFactory {
public:
    enum class LoggerType {
        CONSOLE,
        FILE,
        NONE
    };
    
    static std::unique_ptr<IEventLogger> Create(LoggerType type, const std::string& filename = "game.log");
    static LoggerType ParseArgs(int argc, char* argv[], std::string& out_filename);
};
