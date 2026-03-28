#ifndef GAME_ILOGGER_H
#define GAME_ILOGGER_H

#include <string>

namespace Game {

// Интерфейс для конкретных логгеров (файл, консоль)
class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(const std::string& message) = 0;
};

}

#endif
