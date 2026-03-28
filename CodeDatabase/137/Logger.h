#ifndef LOGGER_H
#define LOGGER_H

#include "GameEvent.h"
#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include <vector>
#include <functional>

class ILogWriter {
public:
    virtual ~ILogWriter() = default;
    virtual void Write(const std::string& message) = 0;
    virtual void Flush() = 0;
};

class FileLogWriter : public ILogWriter {
public:
    explicit FileLogWriter(const std::string& filename);
    ~FileLogWriter() override;

    void Write(const std::string& message) override;
    void Flush() override;

private:
    std::ofstream file_;
};

class ConsoleLogWriter : public ILogWriter {
public:
    void Write(const std::string& message) override;
    void Flush() override;
};

class Logger {
public:
    Logger();
    ~Logger();

    void AddWriter(std::unique_ptr<ILogWriter> writer);
    void Log(const GameEvent& event);
    void Log(const std::string& message);
    void SetEnabled(bool enabled);
    bool IsEnabled() const;

private:
    std::vector<std::unique_ptr<ILogWriter>> writers_;
    bool enabled_;
};

class EventObserver {
public:
    virtual ~EventObserver() = default;
    virtual void OnEvent(const GameEvent& event) = 0;
};

class LoggerObserver : public EventObserver {
public:
    explicit LoggerObserver(Logger& logger);
    void OnEvent(const GameEvent& event) override;

private:
    Logger& logger_;
};

#endif // LOGGER_H
