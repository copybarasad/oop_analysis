#pragma once
#include "IGameLogger.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

// Логгер в консоль
class ConsoleLogger : public IGameLogger {
private:
    std::string getCurrentTime() const {
        auto now = std::time(nullptr);
        auto* timeinfo = std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(timeinfo, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
    
public:
    void logEvent(const GameEvent& event) override {
        std::cout << "[" << getCurrentTime() << "] " << event.description << std::endl;
    }
};

