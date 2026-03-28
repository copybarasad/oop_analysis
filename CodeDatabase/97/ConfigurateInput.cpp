#include "ConfigurateInput.h"
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <iostream>


static std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end   = s.find_last_not_of(" \t\r\n");
    if (start == std::string::npos || end == std::string::npos)
        return "";
    return s.substr(start, end - start + 1);
}

static const std::unordered_map<std::string, CommandType> kNameToCommand = {
        {"MOVE_UP",     CommandType::MoveUp},
        {"MOVE_DOWN",   CommandType::MoveDown},
        {"MOVE_LEFT",   CommandType::MoveLeft},
        {"MOVE_RIGHT",  CommandType::MoveRight},
        {"SWITCH_MODE", CommandType::SwitchMode},
        {"SHOW_HAND",   CommandType::ShowHand},
        {"CAST",        CommandType::Cast},
        {"QUIT",        CommandType::Quit}
};

ConfigurateInput::ConfigurateInput(const std::string& cfgPath) {
    setDefault();

    if (!cfgPath.empty()) {
        loadConfig(cfgPath);
        if (!validate()) {
            std::cout << "[Input] Invalid controls.cfg → using default controls\n";
            setDefault();
        }
    }
}

void ConfigurateInput::setDefault() {
    keyToCommand_.clear();

    keyToCommand_['w'] = CommandType::MoveUp;
    keyToCommand_['s'] = CommandType::MoveDown;
    keyToCommand_['a'] = CommandType::MoveLeft;
    keyToCommand_['d'] = CommandType::MoveRight;
    keyToCommand_['t'] = CommandType::SwitchMode;
    keyToCommand_['h'] = CommandType::ShowHand;
    keyToCommand_['c'] = CommandType::Cast;
    keyToCommand_['q'] = CommandType::Quit;
}

void ConfigurateInput::loadConfig(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "[Input] controls.cfg not found\n";
        return;
    }
    keyToCommand_.clear();
    bool error = false;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            error = true;
            break;
        }
        std::istringstream ss(line);
        std::string name, value;
        if (!std::getline(ss, name, '=')) {
            error = true;
            break;
        }
        if (!std::getline(ss, value)) {
            error = true;
            break;
        }
        name  = trim(name);
        value = trim(value);
        if (value.empty()) {
            error = true;
            break;
        }
        if (value.size() != 1) {
            error = true;
            break;
        }
        char key = value[0];
        if (std::isspace(static_cast<unsigned char>(key))) {
            error = true;
            break;
        }
        auto it = kNameToCommand.find(name);
        if (it == kNameToCommand.end()) {
            error = true;
            break;
        }
        keyToCommand_[key] = it->second;
    }
    if (error) {
        keyToCommand_.clear();
    }
}



bool ConfigurateInput::validate() const {
    std::unordered_set<CommandType> usedCommands;
    std::unordered_set<char> usedKeys;

    for (const auto& [key, cmd] : keyToCommand_) {
        if (usedKeys.count(key)) return false;
        if (usedCommands.count(cmd)) return false;

        usedKeys.insert(key);
        usedCommands.insert(cmd);
    }

    for (const auto& [_, cmd] : kNameToCommand) {
        if (!usedCommands.count(cmd))
            return false;
    }

    return true;
}

Command ConfigurateInput::readCommand() {
    std::string line;
    std::getline(std::cin, line);

    Command cmd;
    if (line.empty())
        return cmd;

    char key = line[0];
    auto it = keyToCommand_.find(key);
    if (it == keyToCommand_.end())
        return cmd;

    cmd.type = it->second;

    if (cmd.type == CommandType::Cast) {
        std::istringstream ss(line.substr(1));
        ss >> cmd.castIndex;

        int r, c;
        if (ss >> r >> c)
            cmd.target = Coord{r, c};
    }

    return cmd;
}

