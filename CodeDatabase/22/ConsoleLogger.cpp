#include "logger/ConsoleLogger.h"

void ConsoleLogger::log(const Event &event)
{
    std::cout << "[LOG] " << event.message << std::endl;
}