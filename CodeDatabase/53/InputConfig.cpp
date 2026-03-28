#include "InputConfig.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>

const std::unordered_set<std::string> VALID_COMMANDS = {
    "MOVE_UP", "MOVE_DOWN", "MOVE_LEFT", "MOVE_RIGHT",
    "ATTACK", "SWITCH_ATTACK", "USE_SPELL", "OPEN_SHOP",
    "QUIT", "SAVE", "LOAD"
};

InputConfig::InputConfig() {
    setDefaults();
}

void InputConfig::setDefaults() {
    keyToCommand.clear();
    commandToKey.clear();
    
    commandToKey["MOVE_UP"] = 'W';
    commandToKey["MOVE_DOWN"] = 'S';
    commandToKey["MOVE_LEFT"] = 'A';
    commandToKey["MOVE_RIGHT"] = 'D';
    commandToKey["ATTACK"] = 'F';
    commandToKey["SWITCH_ATTACK"] = 'R';
    commandToKey["USE_SPELL"] = 'E';
    commandToKey["OPEN_SHOP"] = 'B';
    commandToKey["QUIT"] = 'Q';
    commandToKey["SAVE"] = 'Z';
    commandToKey["LOAD"] = 'L';
    
    for (const auto& pair : commandToKey) {
        keyToCommand[pair.second] = pair.first;
    }
}

bool InputConfig::isValidConfig() const {
    std::unordered_set<char> usedKeys;
    
    for (const auto& pair : commandToKey) {
        if (VALID_COMMANDS.find(pair.first) == VALID_COMMANDS.end()) {
            std::cerr << "Неизвестная команда: " << pair.first << std::endl;
            return false;
        }
        
        if (usedKeys.find(pair.second) != usedKeys.end()) {
            std::cerr << "Клавиша '" << pair.second << "' назначена на несколько команд" << std::endl;
            return false;
        }
        usedKeys.insert(pair.second);
    }
    
    for (const auto& command : VALID_COMMANDS) {
        if (commandToKey.find(command) == commandToKey.end()) {
            std::cerr << "Отсутствует команда: " << command << std::endl;
            return false;
        }
    }
    
    return true;
}

bool InputConfig::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл конфигурации: " << filename 
                  << ". Используются настройки по умолчанию." << std::endl;
        return false;
    }
    
    std::map<std::string, char> tempCommandToKey;
    std::string line;
    
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        std::istringstream iss(line);
        std::string command;
        std::string keyStr;
        
        if (std::getline(iss, command, '=') && std::getline(iss, keyStr)) {
            command.erase(0, command.find_first_not_of(" \t"));
            command.erase(command.find_last_not_of(" \t") + 1);
            keyStr.erase(0, keyStr.find_first_not_of(" \t"));
            keyStr.erase(keyStr.find_last_not_of(" \t") + 1);
            
            if (command.empty() || keyStr.empty()) {
                continue;
            }
            
            char key = toupper(keyStr[0]);
            tempCommandToKey[command] = key;
        }
    }
    
    file.close();
    
    commandToKey = tempCommandToKey;
    
    keyToCommand.clear();
    for (const auto& pair : commandToKey) {
        keyToCommand[pair.second] = pair.first;
    }
    
    if (!isValidConfig()) {
        std::cerr << "Конфигурация невалидна. Используются настройки по умолчанию." << std::endl;
        setDefaults();
        return false;
    }
    
    return true;
}

bool InputConfig::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << "# Конфигурация управления\n";
    file << "# Формат: КОМАНДА=КЛАВИША\n";
    file << "# Доступные команды:\n";
    file << "# MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT - движение\n";
    file << "# ATTACK - атака\n";
    file << "# SWITCH_ATTACK - смена типа атаки\n";
    file << "# USE_SPELL - использование заклинания\n";
    file << "# OPEN_SHOP - магазин\n";
    file << "# QUIT - выход\n";
    file << "# SAVE - сохранение\n";
    file << "# LOAD - загрузка\n\n";
    
    for (const auto& pair : commandToKey) {
        file << pair.first << "=" << pair.second << "\n";
    }
    
    file.close();
    return true;
}

std::string InputConfig::getCommandForKey(char key) const {
    key = toupper(key);
    auto it = keyToCommand.find(key);
    if (it != keyToCommand.end()) {
        return it->second;
    }
    return "";
}

char InputConfig::getKeyForCommand(const std::string& command) const {
    auto it = commandToKey.find(command);
    if (it != commandToKey.end()) {
        return it->second;
    }
    return '\0';
}

std::unordered_set<std::string> InputConfig::getAllCommands() const {
    std::unordered_set<std::string> commands;
    for (const auto& pair : commandToKey) {
        commands.insert(pair.first);
    }
    return commands;
}

std::unordered_set<char> InputConfig::getAllKeys() const {
    std::unordered_set<char> keys;
    for (const auto& pair : keyToCommand) {
        keys.insert(pair.first);
    }
    return keys;
}