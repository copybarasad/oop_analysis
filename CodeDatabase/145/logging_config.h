#ifndef LOGGING_CONFIG_H
#define LOGGING_CONFIG_H

#include <string>
#include <vector>

struct LoggingConfig {
    enum class LogTarget {
        CONSOLE,
        FILE,
        BOTH
    };
    
    LogTarget target;
    std::string logFilename;
    bool enabled;
    
    LoggingConfig()
        : target(LogTarget::CONSOLE), logFilename("game.log"), enabled(true) {}
};

class LoggingManager {
public:
    static LoggingConfig parseArgs(int argc, char* argv[]);
    static void initializeLogging(const LoggingConfig& config);
};

#endif