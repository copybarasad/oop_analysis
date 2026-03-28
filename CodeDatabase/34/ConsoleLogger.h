#ifndef GAME_CONSOLELOGGER_H
#define GAME_CONSOLELOGGER_H

#include "ILogger.h"

namespace Game {

class ConsoleLogger : public ILogger {
public:
    void log(const std::string& message) override;
};

}

#endif
