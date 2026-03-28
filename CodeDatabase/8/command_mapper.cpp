#include "command_mapper.h"
#include <iostream>
CommandMapper::CommandMapper() {
    config.setDefaultConfig();
}

bool CommandMapper::initialize(const std::string& configFile) {
    if (configFile.empty()) {
        return true;
    }
    if (config.loadFromFile(configFile)) {
        std::cout << "Управление загружено из: " << configFile << std::endl;
        return true;
    } else {
        std::cout << "Используются настройки управления по умолчанию" << std::endl;
        return false;
    }
}

std::string CommandMapper::mapInputToCommand(const std::string& input) const {
    return config.getCommandForKey(input);
}

const ControlConfig& CommandMapper::getConfig() const {
    return config;
}

std::string CommandMapper::getFormattedCommand(const std::string& commandName) const {
    std::string key = config.getKeyForCommand(commandName);
    if (key.empty()) {
        return commandName;
    } else {
        return key;
    }
}