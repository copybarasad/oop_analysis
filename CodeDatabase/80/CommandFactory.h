#pragma once
#ifndef COMMANDFACTORY_H
#define COMMANDFACTORY_H

#include "ICommand.h"
#include "ControlConfig.h"
#include <string>
#include <unordered_map>
#include <memory>
#include <functional>

class Player;
class Map;
class Game;

class CommandFactory {
private:
    ControlConfig controlConfig;
    std::unordered_map<std::string, std::unique_ptr<ICommand>> commands;

    using CommandAction = bool (*)(Player&, Map&, Game&);
    std::unordered_map<std::string, CommandAction> actionMap;

public:
    CommandFactory();
    explicit CommandFactory(const std::string& configFile); 

    void registerCommand(std::string name,
        std::string description,
        bool (*action)(Player&, Map&, Game&));

    std::unique_ptr<ICommand> createCommand(std::string name);

    std::unique_ptr<ICommand> createCommandFromInput(const std::string& input);

    CommandAction getActionByName(const std::string& name);

    bool hasCommand(std::string name);

    void showAllCommands();

    std::string getControlHelp() const;

    ControlConfig& getControlConfig() { return controlConfig; }
    const ControlConfig& getControlConfig() const { return controlConfig; }

    bool reloadConfig(const std::string& configFile);

private:
    void registerAllCommands();
};

#endif