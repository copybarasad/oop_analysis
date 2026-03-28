#ifndef LOGGER_H
#define LOGGER_H

#include <vector>
#include <string>
class Logger {
public:
    static void clearScreen();
    static void displayMessages();
    static void logMessage(const std::string& message);
    static void clear();

private:
    static std::vector<std::string> messageBuffer;
};

#endif