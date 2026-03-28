#include "logging_config.h"
#include "event_bus.h"
#include "event_logger.h"
#include <iostream>
#include <string>

LoggingConfig LoggingManager::parseArgs(int argc, char* argv[]) {
    LoggingConfig config;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "--log" && i + 1 < argc) {
            std::string value = argv[i + 1];
            i++;
            
            if (value == "console") {
                config.target = LoggingConfig::LogTarget::CONSOLE;
            } else if (value == "file") {
                config.target = LoggingConfig::LogTarget::FILE;
            } else if (value == "both") {
                config.target = LoggingConfig::LogTarget::BOTH;
            } else if (value.find("file:") == 0) {
                config.target = LoggingConfig::LogTarget::FILE;
                config.logFilename = value.substr(5);
            } else {
                std::cerr << "Unknown log target: " << value << ". Using default.\n";
            }
        } else if (arg == "--no-log") {
            config.enabled = false;
        }
    }
    
    return config;
}

void LoggingManager::initializeLogging(const LoggingConfig& config) {
    if (!config.enabled) {
        return;
    }
    
    EventBus& eventBus = EventBus::getInstance();
    
    switch (config.target) {
        case LoggingConfig::LogTarget::CONSOLE:
            eventBus.subscribe(std::make_shared<ConsoleEventLogger>());
            break;
        case LoggingConfig::LogTarget::FILE:
            eventBus.subscribe(std::make_shared<FileEventLogger>(config.logFilename));
            break;
        case LoggingConfig::LogTarget::BOTH:
            eventBus.subscribe(std::make_shared<ConsoleEventLogger>());
            eventBus.subscribe(std::make_shared<FileEventLogger>(config.logFilename));
            break;
    }
}