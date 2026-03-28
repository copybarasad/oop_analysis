#pragma once

#include <memory>
#include <fstream>
#include <iostream>
#include <string>

class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(const std::string& message) = 0;
};

class ConsoleLogger : public ILogger {
public:
    void log(const std::string& message) override {
        std::cout << message << std::endl;
    }
};

class FileLogger : public ILogger {
public:
    explicit FileLogger(const std::string& path) : _out(path, std::ios::app) {}
    void log(const std::string& message) override {
        if (_out.is_open()) {
            _out << message << std::endl;
        }
    }
private:
    std::ofstream _out;
};

class EventBus {
public:
    explicit EventBus(std::shared_ptr<ILogger> logger = nullptr) : _logger(std::move(logger)) {}

    void setLogger(std::shared_ptr<ILogger> logger) { _logger = std::move(logger); }

    void publish(const std::string& message) const {
        if (_logger) {
            _logger->log(message);
        }
    }

private:
    std::shared_ptr<ILogger> _logger;
};
