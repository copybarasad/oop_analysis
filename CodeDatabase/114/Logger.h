#ifndef LOGGER_H
#define LOGGER_H

#include "IObserver.h"
#include <string>
#include <fstream>
#include <memory>
#include <vector>

enum class LogOutputType {
    FILE,
    CONSOLE
};

class Logger : public IObserver {
private:
    std::vector<LogOutputType> outputTypes;
    std::string logFilePath;
    std::unique_ptr<std::ofstream> logFile;
    
    Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    void writeToFile(const std::string& message);
    void writeToConsole(const std::string& message);
    void initializeFile();
    bool hasOutputType(LogOutputType type) const;

public:
    ~Logger();
    
    static Logger& getInstance();
    
    void initialize(const std::vector<LogOutputType>& types, const std::string& filePath = "game.log");
    void onEvent(const GameEvent& event) override;
    void log(const std::string& message);
    void close();
};

#endif

