#include "control_config.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>

ControlConfig::ControlConfig() {
    setDefaultConfig();
}

bool ControlConfig::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Файл конфигурации " << filename << " не найден. Используются настройки по умолчанию." << std::endl;
        return false;
    }
    commandToKey.clear();
    keyToCommand.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        std::istringstream iss(line);
        std::string command, key;
        
        if (iss >> command >> key) {
            commandToKey[command] = key;
            keyToCommand[key] = command;
        }
    }
    
    file.close();
    if (!isValid()) {
        std::cout << "Некорректная конфигурация в файле " << filename << "." << std::endl;
        setDefaultConfig();
        return false;
    }
    
    std::cout << "Конфигурация управления загружена из " << filename << std::endl;
    return true;
}

void ControlConfig::setDefaultConfig() {
    commandToKey = defaultConfig;
    keyToCommand.clear();
    for (const auto& pair : defaultConfig) {
        keyToCommand[pair.second] = pair.first;
    }
}

bool ControlConfig::isValid() const {
    for (const auto& defaultPair : defaultConfig) {
        if (commandToKey.find(defaultPair.first) == commandToKey.end()) {
            std::cout << "Отсутствует назначение для команды: " << defaultPair.first << std::endl;
            return false;
        }
    }
    if (keyToCommand.size() != commandToKey.size()) {
        std::cout << "Обнаружены дублирующиеся назначения клавиш" << std::endl;
        return false;
    }
    std::unordered_set<std::string> usedCommands;
    for (const auto& pair : commandToKey) {
        if (!usedCommands.insert(pair.first).second) {
            return false;
        }
    }
    return true;
}

std::string ControlConfig::getKeyForCommand(const std::string& command) const {
    auto it = commandToKey.find(command);
    if (it != commandToKey.end()) {
        return it->second;
    }
    return "";
}
    
std::string ControlConfig::getCommandForKey(const std::string& key) const {
    auto it = keyToCommand.find(key);
    if (it != keyToCommand.end()) {
        return it->second;
    }
    return "";
}