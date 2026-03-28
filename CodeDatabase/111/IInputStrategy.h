#ifndef IINPUT_STRATEGY_H
#define IINPUT_STRATEGY_H

#include "PlayerCommand.h"
#include <string>

// Интерфейс для стратегии ввода
class IInputStrategy {
public:
    virtual ~IInputStrategy() = default;
    virtual PlayerCommand getCommand() = 0;
    virtual void showHelp() = 0;
    virtual void loadKeyBindingsFromFile(const std::string& filename) = 0;
};

#endif // IINPUT_STRATEGY_H