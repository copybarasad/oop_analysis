#ifndef CONSOLE_INPUT_STRATEGY_H
#define CONSOLE_INPUT_STRATEGY_H

#include "IInputStrategy.h"
#include "CommandFactory.h"
#include <memory>

class ConsoleInputStrategy : public IInputStrategy {
private:
    CommandFactory commandFactory;
    
public:
    ConsoleInputStrategy();
    std::unique_ptr<ICommand> createCommand(char input, GameController& controller) override;
    void loadConfig(const InputConfig& config) override;
    void showHelp() const override;
    bool isValidInput(char input) const override;
    void loadConfigFromFile(const std::string& filename);
};

#endif