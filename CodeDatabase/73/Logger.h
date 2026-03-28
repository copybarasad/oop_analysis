#ifndef GAMELB1_LOGGER_H
#define GAMELB1_LOGGER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(const std::string& message) = 0;
};

class ConsoleLogger : public ILogger {
public:
    void log(const std::string& message) override {
        std::cout << "[LOG]: " << message << std::endl;
    }
};

class FileLogger : public ILogger {
    std::ofstream ofs;
public:
    explicit FileLogger(const std::string& filename) {
        ofs.open(filename, std::ios::app); // Дописываем в конец
    }
    ~FileLogger() override {
        if (ofs.is_open()) ofs.close();
    }
    void log(const std::string& message) override {
        if (ofs.is_open()) {
            ofs << "[LOG]: " << message << std::endl;
        }
    }
};

class Observable {
    std::vector<ILogger*> observers;
public:
    void addObserver(ILogger* observer) {
        observers.push_back(observer);
    }

    void notifyObservers(const std::string& message) {
        for (auto* obs : observers) {
            obs->log(message);
        }
    }
};

#endif //GAMELB1_LOGGER_H