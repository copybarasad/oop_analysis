#include "ConsoleLogger.h"
#include <iostream>

void ConsoleLogger::Log(const std::string& message) {
    std::cout << message << '\n';
}
