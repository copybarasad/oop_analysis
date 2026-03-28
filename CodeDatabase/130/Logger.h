#ifndef LOGGER_H
#define LOGGER_H

#include "GameEvent.h"
#include <fstream>
#include <memory>

class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(const GameEvent& event) = 0;
};

class ConsoleLogger : public ILogger {
public:
    void log(const GameEvent& event) override;
};

class FileLogger : public ILogger {
public:
    explicit FileLogger(const std::string& path);
    void log(const GameEvent& event) override;

private:
    std::ofstream out_;
};

class LoggerFactory {
public:
    static std::shared_ptr<ILogger> createConsole();
    static std::shared_ptr<ILogger> createFile(const std::string& path);
};

#endif


