//
// Created by Artem on 24.12.2025.
//

#include "Logger.h"

std::unique_ptr<Logger> Logger::instance = nullptr;

std::vector<std::string> Logger::getLastMessages() {
    {
        auto& logger = getInstance();
        return std::vector<std::string>(logger.buff.begin(),
                                        logger.buff.end());
    }
}

void Logger::tech(const std::string &message) {
    getInstance().logInternal("TECH", message);
}

void Logger::info(const std::string &message)  {
    getInstance().logInternal("INFO", message);
}

void Logger::init(LogOutput output, const std::string &filename)  {
    auto& logger = getInstance();

    if (logger.isInitialized) {
        std::cout << "Logger already initialized!" << std::endl;
        return;
    }

    logger.outputType = output;

    if (output == LogOutput::FILE) {
        if (filename.empty()) {
            std::cout << "Filename required for file output!" << std::endl;
            return;
        }

        logger.logFile.open(filename, std::ios::out | std::ios::app);
        if (!logger.logFile.is_open()) {
            std::cout << "Cannot open log file: " << filename << std::endl;
            return;
        }
    }

    logger.isInitialized = true;
    tech("Logger initialized");
}

Logger &Logger::getInstance() {
    if (!instance) {
        instance.reset(new Logger());
    }
    return *instance;
}

void Logger::shutdown() {
    if (instance) {
        instance->isInitialized = false;
        if (instance->logFile.is_open()) {
            instance->logFile.close();
        }
        instance->buff.clear();
    }
}

void Logger::addToBuffer(const std::string &message) {
    if (buff.size() >= 10) {
        buff.pop_front();
    }
    buff.push_back(message);
}

void Logger::logInternal(const std::string &level, const std::string &message) {
    if (!isInitialized) {
        std::cout << "Logger not initialized" << std::endl;
        return;
    }

    std::string logMessage = "[" + level + "] " + message;

    if(level == "INFO"){
        addToBuffer(message);
    }


    switch (outputType) {
        case LogOutput::CONSOLE:
            std::cout << logMessage << std::endl;
            break;

        case LogOutput::FILE:
            if (logFile.is_open()) {
                logFile << logMessage << std::endl;
            }
            break;
    }
}