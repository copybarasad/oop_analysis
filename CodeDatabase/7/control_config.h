#ifndef CONTROL_CONFIG_H
#define CONTROL_CONFIG_H

#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <set>

class ControlConfig {
private:
    std::unordered_map<std::string, std::string> keyToCommand;
    std::unordered_map<std::string, std::string> commandToKey;
    
public:
    ControlConfig() {
        setDefaultConfig();
    }
    
    void setDefaultConfig() {
        keyToCommand.clear();
        commandToKey.clear();
        keyToCommand = {
            {"w", "MOVE_UP"}, {"s", "MOVE_DOWN"}, {"a", "MOVE_LEFT"}, {"d", "MOVE_RIGHT"},
            {"k", "ATTACK"}, {"t", "SWITCH_COMBAT"}, {"c", "CAST_SPELL"}, {"b", "BUY_SPELL"},
            {"save", "SAVE"}, {"load", "LOAD"}, {"status", "SHOW_STATUS"}, {"q", "QUIT"}
        };
        for (const auto& pair : keyToCommand) {
            commandToKey[pair.second] = pair.first;
        }
    }
    
    bool loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::unordered_map<std::string, std::string> tempKeys;
    std::unordered_map<std::string, std::string> tempCommands;
    std::string line;
    
    std::vector<std::string> requiredCommands = {
        "MOVE_UP", "MOVE_DOWN", "MOVE_LEFT", "MOVE_RIGHT",
        "ATTACK", "SWITCH_COMBAT", "CAST_SPELL", "BUY_SPELL",
        "SAVE", "LOAD", "SHOW_STATUS", "QUIT"
    };

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        std::string key, command;
        
        if (!(iss >> key >> command)) continue; 

        if (tempKeys.count(key) > 0 || tempCommands.count(command) > 0) {
            std::cout << "[ОШИБКА] Обнаружен дубликат клавиши или команды!" << std::endl;
            setDefaultConfig();
            return false;
        }
        tempKeys[key] = command;
        tempCommands[command] = key;
    }

    for (const std::string& req : requiredCommands) {
        if (tempCommands.count(req) == 0) {
            std::cout << "[ОШИБКА] В конфиге отсутствует команда: " << req << std::endl;
            setDefaultConfig();
            return false;
        }
    }
    
    keyToCommand = tempKeys;
    commandToKey = tempCommands;
    return true;
}

    std::string getCommandForKey(const std::string& key) const {
        auto it = keyToCommand.find(key);
        return (it != keyToCommand.end()) ? it->second : "";
    }

    std::string getKeyForCommand(const std::string& command) const {
        auto it = commandToKey.find(command);
        return (it != commandToKey.end()) ? it->first : "";
    }
};

#endif