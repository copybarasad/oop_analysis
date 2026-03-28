#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(const std::string& message) = 0;
};

class IObservable {
public:
    void addObserver(ILogger* observer) {
        observers_.push_back(observer);
    }
    
    void removeObserver(ILogger* observer) {
        observers_.erase(std::remove(observers_.begin(), observers_.end(), observer), observers_.end());
    }

protected:
    void notifyObservers(const std::string& message) const {
        for (auto* obs : observers_) {
            obs->log(message);
        }
    }

private:
    std::vector<ILogger*> observers_;
};

class ConsoleLogger : public ILogger {
public:
    void log(const std::string& message) override {
        std::cout << "[LOG]: " << message << std::endl;
    }
};

class FileLogger : public ILogger {
public:
    explicit FileLogger(const std::string& filename) {
        ofs_.open(filename, std::ios::app);
    }
    
    ~FileLogger() {
        if (ofs_.is_open()) ofs_.close();
    }

    void log(const std::string& message) override {
        if (ofs_.is_open()) {
            ofs_ << "[LOG]: " << message << std::endl;
        }
    }

private:
    std::ofstream ofs_;
};

#endif