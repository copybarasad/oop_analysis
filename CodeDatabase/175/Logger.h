#pragma once
#include "EventSystem.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <ctime>
#include <sstream>
#include <iomanip>

enum class LogOutput {
    CONSOLE,
    FILE,
    BOTH
};

class Logger : public EventObserver {
private:
    std::ofstream logFile;
    LogOutput outputType;
    std::string filename;
    
public:
    Logger(LogOutput output = LogOutput::BOTH, const std::string& file = "game.log") 
        : outputType(output), filename(file) {
        
        if (outputType == LogOutput::FILE || outputType == LogOutput::BOTH) {
            logFile.open(filename, std::ios::app);
        }
    }
    
    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }
    
    void onEvent(const GameEvent& event) override {
        std::string logMessage = event.toString();
        
        switch(outputType) {
            case LogOutput::CONSOLE:
                std::cout << logMessage << std::endl;
                break;
            case LogOutput::FILE:
                if (logFile.is_open()) {
                    logFile << logMessage << std::endl;
                }
                break;
            case LogOutput::BOTH:
                std::cout << logMessage << std::endl;
                if (logFile.is_open()) {
                    logFile << logMessage << std::endl;
                }
                break;
        }
    }
};