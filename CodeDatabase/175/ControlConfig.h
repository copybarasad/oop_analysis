#pragma once
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

struct KeyBinding {
    char key;
    std::string command;
    std::string description;
    
    KeyBinding(char k, const std::string& cmd, const std::string& desc = "")
        : key(k), command(cmd), description(desc) {}
};

class ControlConfig {
private:
    std::map<char, std::string> keyBindings;
    std::map<std::string, std::vector<char>> commandToKeys;
    std::vector<KeyBinding> defaultBindings;
    
    void loadDefaults() {
        defaultBindings = {
            {'w', "MOVE_UP", "Движение вверх"},
            {'s', "MOVE_DOWN", "Движение вниз"},
            {'a', "MOVE_LEFT", "Движение влево"},
            {'d', "MOVE_RIGHT", "Движение вправо"},
            {'m', "SWITCH_ATTACK_MODE", "Сменить режим атаки"},
            {'c', "USE_SPELL", "Использовать заклинание"},
            {'e', "END_TURN", "Закончить ход"},
            {'q', "SAVE_AND_EXIT", "Сохранить и выйти"},
            {'l', "LOAD_GAME", "Загрузить игру"}
        };
        
        for (const auto& binding : defaultBindings) {
            keyBindings[binding.key] = binding.command;
            commandToKeys[binding.command].push_back(binding.key);
        }
    }
    
    bool validateBindings() const {
        std::map<std::string, int> commandCount;
        for (const auto& [key, command] : keyBindings) {
            commandCount[command]++;
            if (commandCount[command] > 1) {
                return false;
            }
        }
        return true;
    }
    
public:
    ControlConfig() {
        loadDefaults();
    }
    
    ControlConfig(const std::string& configFile) {
        loadDefaults();
        loadFromFile(configFile);
    }
    
    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        
        std::string line;
        bool hasErrors = false;
        
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            
            size_t eqPos = line.find('=');
            if (eqPos == std::string::npos) {
                hasErrors = true;
                continue;
            }
            
            std::string keyStr = line.substr(0, eqPos);
            std::string command = line.substr(eqPos + 1);
            
            keyStr.erase(std::remove_if(keyStr.begin(), keyStr.end(), ::isspace), keyStr.end());
            command.erase(std::remove_if(command.begin(), command.end(), ::isspace), command.end());
            
            if (keyStr.length() != 1) {
                hasErrors = true;
                continue;
            }
            
            char key = std::tolower(keyStr[0]);
            bindKey(key, command);
        }
        
        file.close();
        
        if (hasErrors || !validateBindings()) {
            keyBindings.clear();
            commandToKeys.clear();
            loadDefaults();
            return false;
        }
        
        return true;
    }
    
    bool saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        
        file << "# Конфигурация управления\n";
        file << "# Формат: ключ=команда\n\n";
        
        for (const auto& [key, command] : keyBindings) {
            file << key << "=" << command << "\n";
        }
        
        file.close();
        return true;
    }
    
    std::string getCommandForKey(char key) const {
        auto it = keyBindings.find(std::tolower(key));
        if (it != keyBindings.end()) {
            return it->second;
        }
        return "";
    }
    
    std::vector<char> getKeysForCommand(const std::string& command) const {
        auto it = commandToKeys.find(command);
        if (it != commandToKeys.end()) {
            return it->second;
        }
        return {};
    }
    
    void bindKey(char key, const std::string& command) {
        char lowerKey = std::tolower(key);
        
        if (auto oldIt = keyBindings.find(lowerKey); oldIt != keyBindings.end()) {
            std::string oldCommand = oldIt->second;
            auto& keys = commandToKeys[oldCommand];
            keys.erase(std::remove(keys.begin(), keys.end(), lowerKey), keys.end());
        }
        
        keyBindings[lowerKey] = command;
        commandToKeys[command].push_back(lowerKey);
    }
    
    void unbindKey(char key) {
        char lowerKey = std::tolower(key);
        auto it = keyBindings.find(lowerKey);
        if (it != keyBindings.end()) {
            std::string command = it->second;
            keyBindings.erase(it);
            
            auto& keys = commandToKeys[command];
            keys.erase(std::remove(keys.begin(), keys.end(), lowerKey), keys.end());
        }
    }
    
    const std::map<char, std::string>& getAllBindings() const { return keyBindings; }
};