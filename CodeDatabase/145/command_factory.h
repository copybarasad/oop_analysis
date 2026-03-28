#ifndef COMMAND_FACTORY_H
#define COMMAND_FACTORY_H

#include "command.h"
#include "game_config.h"
#include <memory>

class CommandFactory {
private:
    const GameConfig& config;

public:
    explicit CommandFactory(const GameConfig& cfg) : config(cfg) {}
    std::unique_ptr<Command> createCommand(char inputChar);
    const GameConfig& getConfig() const { return config; }
};

#endif