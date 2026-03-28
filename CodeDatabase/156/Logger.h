//
// Created by Artem on 24.12.2025.
//

#ifndef LABAOOP2_LOGGER_H
#define LABAOOP2_LOGGER_H
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <vector>
#include <deque>

enum class LogOutput {
    CONSOLE,
    FILE
};

class Logger {
public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger& getInstance();

    static void init(LogOutput output, const std::string& filename = "");

    static void info(const std::string& message);

    static void tech(const std::string& message);

    static std::vector<std::string> getLastMessages();

    static void shutdown();

    ~Logger() {
        shutdown();
    }


private:
    static std::unique_ptr<Logger> instance;
    std::deque<std::string> buff;
    LogOutput outputType;
    std::ofstream logFile;
    bool isInitialized;

    Logger() : isInitialized(false) {}

    void addToBuffer(const std::string& message);

    void logInternal(const std::string& level, const std::string& message);
};


#endif //LABAOOP2_LOGGER_H
