#ifndef COMMAND_FACTORY_H
#define COMMAND_FACTORY_H

#include "Command.h"
#include "InputConfig.h"
#include <memory>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <limits>

class CommandFactory {
private:
    InputConfig config;
    std::unique_ptr<ICommand> createCommandFromType(CommandType type, GameController& controller);
    std::unique_ptr<ICommand> createAttackCommand(GameController& controller);
    std::unique_ptr<ICommand> createUseSpellCommand(GameController& controller);
    std::unique_ptr<ICommand> createUpgradeSpellCommand(GameController& controller);
    
public:
    CommandFactory();
    void loadConfig(const std::string& filename);
    std::unique_ptr<ICommand> createCommand(char input, GameController& controller);
    bool isValidInput(char input) const;
    void showHelp() const;
    void printConfig() const;
};

#endif