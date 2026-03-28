#include "consolelogger.h"

ConsoleLogger::ConsoleLogger(LogLevel minLevel) : minLevel_(minLevel) {}

void ConsoleLogger::onEvent(const std::shared_ptr<GameEvent>& event)  {
    std::cout << event->toFormattedString() << std::endl;
}

LogLevel ConsoleLogger::getMinLogLevel() const  { return minLevel_; }

void ConsoleLogger::setMinLevel(LogLevel level) { minLevel_ = level; }
