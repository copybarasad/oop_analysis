#ifndef IINPUT_STRATEGY_H
#define IINPUT_STRATEGY_H

#include <memory>
#include "Command.h"
#include "InputConfig.h"

class GameController;

class IInputStrategy {
public:
    virtual ~IInputStrategy() = default;
    
    virtual std::unique_ptr<ICommand> createCommand(char input, GameController& controller) = 0;
    
    virtual void loadConfig(const InputConfig& config) = 0;
    
    virtual void showHelp() const = 0;
    
    virtual bool isValidInput(char input) const = 0;
};

#endif