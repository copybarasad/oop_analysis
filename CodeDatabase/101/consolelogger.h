#ifndef CONSOLELOGGER_H
#define CONSOLELOGGER_H

#include "eventlistener.h"
#include <fstream>
#include <iostream>
#include "gameevent.h"

class ConsoleLogger : public EventListener {
private:
    LogLevel minLevel_;

public:
    ConsoleLogger(LogLevel minLevel = LogLevel::INFO);

    void onEvent(const std::shared_ptr<GameEvent>& event) override;

    LogLevel getMinLogLevel() const override;

    void setMinLevel(LogLevel level);
};

#endif // CONSOLELOGGER_H
