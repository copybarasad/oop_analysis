#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include <limits>
#include <cctype>
#include <fstream>
#include <climits>  
#include "GameCommand.h"

class ConsoleInputProvider {
private:
    std::unordered_map<char, std::string> keyMap;
    std::unordered_map<std::string, GameCommand::Type> commandTypeMap;
    
    void initializeCommandTypeMap() {
        commandTypeMap = {
            {"MOVE_UP", GameCommand::Type::MOVE_UP},
            {"MOVE_DOWN", GameCommand::Type::MOVE_DOWN},
            {"MOVE_LEFT", GameCommand::Type::MOVE_LEFT},
            {"MOVE_RIGHT", GameCommand::Type::MOVE_RIGHT},
            {"SHOW_STATS", GameCommand::Type::SHOW_STATS},
            {"SHOW_INSTRUCTIONS", GameCommand::Type::SHOW_INSTRUCTIONS},
            {"SHOW_SPELLS", GameCommand::Type::SHOW_SPELLS},
            {"BUY_SPELLS", GameCommand::Type::BUY_SPELLS},
            {"CAST_SPELL", GameCommand::Type::CAST_SPELL},
            {"CHANGE_COMBAT_RANGE", GameCommand::Type::CHANGE_COMBAT_RANGE},
            {"SAVE_GAME", GameCommand::Type::SAVE_GAME},
            {"LOAD_GAME", GameCommand::Type::LOAD_GAME},
            {"EXIT", GameCommand::Type::EXIT}
        };
    }
    
    void loadDefaultControls() {
        keyMap = {
            {'w', "MOVE_UP"},
            {'s', "MOVE_DOWN"}, 
            {'a', "MOVE_LEFT"},
            {'d', "MOVE_RIGHT"},
            {'z', "SHOW_STATS"},
            {'i', "SHOW_INSTRUCTIONS"},
            {'h', "SHOW_SPELLS"},     
            {'p', "BUY_SPELLS"},
            {'c', "CAST_SPELL"},        
            {'b', "CHANGE_COMBAT_RANGE"},
            {'v', "SAVE_GAME"},
            {'l', "LOAD_GAME"},
            {'e', "EXIT"}
        };
    }
    
    int getValidatedInteger(const std::string& prompt, int min = INT_MIN, int max = INT_MAX) {
        int value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value) {
                if (value >= min && value <= max) {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    return value;
                } else {
                    std::cout << "Value must be between " << min << " and " << max << ". Try again." << std::endl;
                }
            } else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number." << std::endl;
            }
        }
    }

public:
    ConsoleInputProvider() {
        initializeCommandTypeMap();
        loadDefaultControls();
    }
    
    ConsoleInputProvider(const std::string& configFile) {
        initializeCommandTypeMap();
        if (!loadControlsFromFile(configFile)) {
            std::cout << "Using default controls." << std::endl;
            loadDefaultControls();
        }
    }
    
    GameCommand getInput() {
        char input;
        std::cout << "Enter command: ";
        std::cin >> input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        char lowerInput = std::tolower(static_cast<unsigned char>(input));
        auto it = keyMap.find(lowerInput);
        
        if (it != keyMap.end()) {
            const std::string& cmdStr = it->second;
            auto cmdIt = commandTypeMap.find(cmdStr);
            
            if (cmdIt != commandTypeMap.end()) {
                GameCommand::Type type = cmdIt->second;
                
                if (type == GameCommand::Type::CAST_SPELL) {
                    int spellIndex = getValidatedInteger("Enter spell index (0-9): ", 0, 9);
                    return GameCommand(type, "", spellIndex);
                }
                
                if (type == GameCommand::Type::CHANGE_COMBAT_RANGE) {
                    int combatRange = getValidatedInteger("Enter combat range (0-melee, 1-ranged): ", 0, 1);
                    return GameCommand(type, "", -1, combatRange);
                }
                
                return GameCommand(type);
            }
        }
        
        return GameCommand(GameCommand::Type::INVALID, "Unknown command");
    }
    
    void setKeyMap(const std::unordered_map<char, std::string>& newKeyMap) {
        keyMap = newKeyMap;
        std::cout << "Key mapping updated." << std::endl;
    }
    
    std::unordered_map<char, std::string> getKeyMap() const {
        return keyMap;
    }
    
    bool loadControlsFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Cannot open controls file: " << filename << std::endl;
            return false;
        }
        
        std::unordered_map<char, std::string> newMap;
        std::string line;
        
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;

            size_t delimiterPos = line.find('=');
            if (delimiterPos == std::string::npos) continue;
            
            std::string keyStr = line.substr(0, delimiterPos);
            std::string command = line.substr(delimiterPos + 1);
            
            keyStr.erase(0, keyStr.find_first_not_of(" \t"));
            keyStr.erase(keyStr.find_last_not_of(" \t") + 1);
            command.erase(0, command.find_first_not_of(" \t"));
            command.erase(command.find_last_not_of(" \t") + 1);
            
            if (keyStr.length() != 1) continue;
            
            char key = std::tolower(static_cast<unsigned char>(keyStr[0]));
            
            if (commandTypeMap.find(command) == commandTypeMap.end()) {
                std::cout << "Unknown command in config: " << command << std::endl;
                file.close();
                return false;
            }
            
            newMap[key] = command;
        }
        
        file.close();
        
        if (!newMap.empty()) {
            keyMap = newMap;
            std::cout << "Controls loaded from: " << filename << std::endl;
            displayControls();
            return true;
        }
        
        return false;
    }
    
    void displayControls() const {
        std::cout << "=== Controls ===" << std::endl;
        for (const auto& pair : keyMap) {
            std::cout << "'" << pair.first << "' -> " << pair.second << std::endl;
        }
        std::cout << "================" << std::endl;
    }
};