//
// Created by Mac on 02.12.2025.
//

#include "ConfigLoader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

std::map<char, CommandType> ConfigLoader::loadFromFile(const std::string& filename) {
    std::map<char, CommandType> config;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Warning: Cannot open config file '" << filename
                  << "'. Using default bindings." << std::endl;
        return config;
    }

    std::map<CommandType, int> commandCount;
    std::string line;
    int lineNum = 0;

    while (std::getline(file, line)) {
        lineNum++;

        line.erase(0, line.find_first_not_of(" \t"));
        if (line.empty() || line[0] == '#') {
            continue;
        }

        size_t equalPos = line.find('=');
        if (equalPos == std::string::npos) {
            std::cerr << "Warning: Line " << lineNum
                      << ": Missing '='. Skipping." << std::endl;
            continue;
        }

        std::string keyStr = line.substr(0, equalPos);
        std::string valueStr = line.substr(equalPos + 1);

        keyStr.erase(0, keyStr.find_first_not_of(" \t"));
        keyStr.erase(keyStr.find_last_not_of(" \t") + 1);
        valueStr.erase(0, valueStr.find_first_not_of(" \t"));
        valueStr.erase(valueStr.find_last_not_of(" \t") + 1);

        char key;
        if (keyStr == "SPACE" || keyStr == "space") {
            key = ' ';
        } else if (keyStr == "ENTER" || keyStr == "enter") {
            key = '\n';
        } else if (keyStr == "TAB" || keyStr == "tab") {
            key = '\t';
        } else if (keyStr == "ESC" || keyStr == "esc") {
            key = 27;
        } else if (keyStr.length() == 1) {
            key = keyStr[0];
        } else {
            std::cerr << "Warning: Line " << lineNum
                      << ": Invalid key '" << keyStr << "'. Skipping." << std::endl;
            continue;
        }


        CommandType command;
        try {
            command = stringToCommand(valueStr);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Warning: Line " << lineNum
                      << ": " << e.what() << " Skipping." << std::endl;
            continue;
        }

        if (config.find(key) != config.end()) {
            std::cerr << "Warning: Line " << lineNum
                      << ": Key '" << keyStr << "' already assigned. "
                      << "Overwriting previous assignment." << std::endl;
        }

        commandCount[command]++;

        config[key] = command;
    }

    file.close();

    if (!validateConfig(config)) {
        std::cerr << "Warning: Config validation failed. Using default bindings." << std::endl;
        return std::map<char, CommandType>();
    }

    return config;
}

CommandType ConfigLoader::stringToCommand(const std::string& str) {
    std::string upperStr = str;
    std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(),
                   [](unsigned char c) { return std::toupper(c); });

    if (upperStr == "MOVE_UP" || upperStr == "UP") {
        return CommandType::MOVE_UP;
    } else if (upperStr == "MOVE_DOWN" || upperStr == "DOWN") {
        return CommandType::MOVE_DOWN;
    } else if (upperStr == "MOVE_LEFT" || upperStr == "LEFT") {
        return CommandType::MOVE_LEFT;
    } else if (upperStr == "MOVE_RIGHT" || upperStr == "RIGHT") {
        return CommandType::MOVE_RIGHT;
    } else if (upperStr == "SWITCH_FIGHT_MODE" || upperStr == "SWITCH" || upperStr == "MODE") {
        return CommandType::SWITCH_FIGHT_MODE;
    } else if (upperStr == "SHOOT" || upperStr == "FIRE" || upperStr == "ATTACK") {
        return CommandType::SHOOT;
    } else if (upperStr == "BUY_SPELL" || upperStr == "BUY" || upperStr == "SHOP") {
        return CommandType::BUY_SPELL;
    } else if (upperStr == "USE_SPELL" || upperStr == "USE" || upperStr == "CAST") {
        return CommandType::USE_SPELL;
    } else if (upperStr == "QUIT_GAME" || upperStr == "QUIT" || upperStr == "EXIT") {
        return CommandType::QUIT_GAME;
    } else if (upperStr == "SAVE_GAME" || upperStr == "SAVE") {
        return CommandType::SAVE_GAME;
    } else {
        throw std::invalid_argument("Unknown command: '" + str + "'");
    }
}

bool ConfigLoader::validateConfig(const std::map<char, CommandType>& config) {

    std::map<CommandType, bool> essentialCommands = {
        {CommandType::MOVE_UP, false},
        {CommandType::MOVE_DOWN, false},
        {CommandType::MOVE_LEFT, false},
        {CommandType::MOVE_RIGHT, false},
        {CommandType::QUIT_GAME, false}
    };

    for (const auto& [key, command] : config) {
        if (essentialCommands.find(command) != essentialCommands.end()) {
            essentialCommands[command] = true;
        }
    }

    for (const auto& [command, isAssigned] : essentialCommands) {
        if (!isAssigned) {
            std::cerr << "Error: Essential command ";
            switch(command) {
                case CommandType::MOVE_UP: std::cerr << "MOVE_UP"; break;
                case CommandType::MOVE_DOWN: std::cerr << "MOVE_DOWN"; break;
                case CommandType::MOVE_LEFT: std::cerr << "MOVE_LEFT"; break;
                case CommandType::MOVE_RIGHT: std::cerr << "MOVE_RIGHT"; break;
                case CommandType::QUIT_GAME: std::cerr << "QUIT_GAME"; break;
                default: break;
            }
            std::cerr << " is not assigned to any key." << std::endl;
            return false;
        }
    }

    std::map<CommandType, std::vector<char>> commandToKeys;
    for (const auto& [key, command] : config) {
        commandToKeys[command].push_back(key);
    }

    for (const auto& [command, keys] : commandToKeys) {
        if (keys.size() > 1) {
            std::cerr << "Warning: Command ";
            std::cerr << " is assigned to multiple keys: ";
            for (char key : keys) {
                if (key == ' ') std::cerr << "SPACE ";
                else if (key == '\n') std::cerr << "ENTER ";
                else std::cerr << "'" << key << "' ";
            }
            std::cerr << std::endl;
        }
    }

    return true;
}