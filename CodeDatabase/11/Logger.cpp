#include "Logger.h"
#include <iostream>
#include <cstdlib>

std::vector<std::string> Logger::messageBuffer;

void Logger::logMessage(const std::string& message) {
    messageBuffer.push_back(message);
    if (messageBuffer.size() > 7) {
        messageBuffer.erase(messageBuffer.begin());
    }
}

void Logger::clear() {
    messageBuffer.clear();
}

void Logger::displayMessages() {
    if (!messageBuffer.empty()) {
        std::cout << "--- Game Log ---" << std::endl;
        for (const auto& msg : messageBuffer) {
            std::cout << " > " << msg << std::endl;
        }
        std::cout << "----------------" << std::endl;
    }
}

void Logger::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}