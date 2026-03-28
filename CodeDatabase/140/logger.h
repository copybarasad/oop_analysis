#pragma once
#include <memory>
#include <string>
#include <iostream>
#include <fstream>

class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(const std::string& msg) = 0;
};

class ConsoleLogger : public ILogger {
public:
    void log(const std::string& msg) override {
        std::cout << "[LOG] " << msg << "\n";
    }
};

class FileLogger : public ILogger {
public:
    explicit FileLogger(const std::string& filename)
        : out(filename, std::ios::app) {}

    void log(const std::string& msg) override {
        if (out) out << "[LOG] " << msg << "\n";
    }
private:
    std::ofstream out;
};

inline std::shared_ptr<ILogger>& global_logger() {
    static std::shared_ptr<ILogger> logger;
    return logger;
}