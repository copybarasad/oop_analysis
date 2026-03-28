#pragma once
#include "IGameLogger.h"
#include <fstream>
#include <iomanip>
#include <ctime>
#include <sstream>

// Логгер в файл
class FileLogger : public IGameLogger {
private:
    std::ofstream logFile_;
    std::string filename_;
    
    std::string getCurrentTime() const {
        auto now = std::time(nullptr);
        auto* timeinfo = std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(timeinfo, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
    
public:
    FileLogger(const std::string& filename) : filename_(filename) {
        logFile_.open(filename, std::ios::app);
        if (!logFile_.is_open()) {
            std::cerr << "Failed to open log file: " << filename << std::endl;
        }
    }
    
    ~FileLogger() {
        close();
    }
    
    void logEvent(const GameEvent& event) override {
        if (logFile_.is_open()) {
            logFile_ << "[" << getCurrentTime() << "] " << event.description << std::endl;
            logFile_.flush();
        }
    }
    
    void close() override {
        if (logFile_.is_open()) {
            logFile_.close();
        }
    }
};

