#ifndef INPUT_CONFIG_H
#define INPUT_CONFIG_H

#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

enum class CommandType {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    ATTACK,
    AREA_ATTACK,
    SWITCH_COMBAT,
    LIST_SPELLS,
    USE_SPELL,
    BUY_SPELL,
    UPGRADE_SPELL,
    HELP,
    SAVE,
    QUIT
};

struct KeyBinding {
    char key;
    CommandType command;
    
    KeyBinding(char k, CommandType cmd) : key(std::tolower(k)), command(cmd) {}
};

class InputConfig {
private:
    std::unordered_map<char, CommandType> keyToCommand;
    std::unordered_map<CommandType, std::vector<char>> commandToKeys;
    
    bool hasDuplicates() const {
        std::unordered_map<char, int> keyCount;
        for (const auto& [key, cmd] : keyToCommand) {
            keyCount[key]++;
            if (keyCount[key] > 1) {
                std::cerr << "Error: Key '" << key << "' is bound to multiple commands" << std::endl;
                return true;
            }
        }
        return false;
    }
    
    bool allCommandsHaveKeys() const {
        std::vector<CommandType> requiredCommands = {
            CommandType::MOVE_UP, CommandType::MOVE_DOWN,
            CommandType::MOVE_LEFT, CommandType::MOVE_RIGHT,
            CommandType::ATTACK, CommandType::AREA_ATTACK,
            CommandType::SWITCH_COMBAT, CommandType::LIST_SPELLS,
            CommandType::USE_SPELL, CommandType::BUY_SPELL,
            CommandType::UPGRADE_SPELL, CommandType::HELP,
            CommandType::SAVE, CommandType::QUIT
        };
        
        for (const auto& cmd : requiredCommands) {
            if (commandToKeys.find(cmd) == commandToKeys.end() || 
                commandToKeys.at(cmd).empty()) {
                std::cerr << "Error: Command type missing key binding" << std::endl;
                return false;
            }
        }
        return true;
    }
    
public:
    InputConfig() = default;
    
    void addBinding(char key, CommandType command) {
        key = std::tolower(key);
        
        auto oldCmdIt = keyToCommand.find(key);
        if (oldCmdIt != keyToCommand.end()) {
            CommandType oldCmd = oldCmdIt->second;
            auto& keys = commandToKeys[oldCmd];
            keys.erase(std::remove(keys.begin(), keys.end(), key), keys.end());
        }
        
        keyToCommand[key] = command;
        commandToKeys[command].push_back(key);
    }
    
    CommandType getCommandForKey(char key) const {
        key = std::tolower(key);
        auto it = keyToCommand.find(key);
        if (it != keyToCommand.end()) {
            return it->second;
        }
        throw std::runtime_error("No command bound to key: " + std::string(1, key));
    }
    
    const std::vector<char>& getKeysForCommand(CommandType command) const {
        static const std::vector<char> empty;
        auto it = commandToKeys.find(command);
        if (it != commandToKeys.end()) {
            return it->second;
        }
        return empty;
    }
    
    bool hasKey(char key) const {
        key = std::tolower(key);
        return keyToCommand.find(key) != keyToCommand.end();
    }
    
    bool validate() const {
        if (hasDuplicates()) {
            return false;
        }
        
        if (!allCommandsHaveKeys()) {
            return false;
        }
        
        return true;
    }
    
    static InputConfig createDefault() {
        InputConfig config;
        
        config.addBinding('w', CommandType::MOVE_UP);
        config.addBinding('s', CommandType::MOVE_DOWN);
        config.addBinding('a', CommandType::MOVE_LEFT);
        config.addBinding('d', CommandType::MOVE_RIGHT);
        
        config.addBinding('t', CommandType::ATTACK);
        config.addBinding('x', CommandType::AREA_ATTACK);
        config.addBinding('c', CommandType::SWITCH_COMBAT);
        
        config.addBinding('l', CommandType::LIST_SPELLS);
        config.addBinding('u', CommandType::USE_SPELL);
        config.addBinding('b', CommandType::BUY_SPELL);
        config.addBinding('g', CommandType::UPGRADE_SPELL);
        
        config.addBinding('h', CommandType::HELP);
        config.addBinding('v', CommandType::SAVE);
        config.addBinding('q', CommandType::QUIT);
        
        return config;
    }
    
    void printConfig() const {
        std::cout << "\n=== Current Input Configuration ===" << std::endl;
        for (const auto& [key, cmd] : keyToCommand) {
            std::cout << "Key '" << key << "' -> ";
            switch (cmd) {
                case CommandType::MOVE_UP: std::cout << "MOVE_UP"; break;
                case CommandType::MOVE_DOWN: std::cout << "MOVE_DOWN"; break;
                case CommandType::MOVE_LEFT: std::cout << "MOVE_LEFT"; break;
                case CommandType::MOVE_RIGHT: std::cout << "MOVE_RIGHT"; break;
                case CommandType::ATTACK: std::cout << "ATTACK"; break;
                case CommandType::AREA_ATTACK: std::cout << "AREA_ATTACK"; break;
                case CommandType::SWITCH_COMBAT: std::cout << "SWITCH_COMBAT"; break;
                case CommandType::LIST_SPELLS: std::cout << "LIST_SPELLS"; break;
                case CommandType::USE_SPELL: std::cout << "USE_SPELL"; break;
                case CommandType::BUY_SPELL: std::cout << "BUY_SPELL"; break;
                case CommandType::UPGRADE_SPELL: std::cout << "UPGRADE_SPELL"; break;
                case CommandType::HELP: std::cout << "HELP"; break;
                case CommandType::SAVE: std::cout << "SAVE"; break;
                case CommandType::QUIT: std::cout << "QUIT"; break;
            }
            std::cout << std::endl;
        }
        std::cout << "==================================" << std::endl;
    }
};

class InputConfigLoader {
public:
    static InputConfig load(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Warning: Could not open config file '" << filename 
                      << "'. Using default configuration." << std::endl;
            return InputConfig::createDefault();
        }
        
        InputConfig config;
        std::string line;
        int lineNum = 0;
        
        while (std::getline(file, line)) {
            lineNum++;
            
            if (line.empty() || line[0] == '#') {
                continue;
            }
            
            std::istringstream iss(line);
            std::string keyStr, commandStr;
            
            if (!(iss >> keyStr >> commandStr)) {
                std::cerr << "Warning: Invalid format on line " << lineNum 
                          << ": " << line << std::endl;
                continue;
            }
            
            if (keyStr.length() != 1) {
                std::cerr << "Warning: Key must be single character on line " 
                          << lineNum << ": " << keyStr << std::endl;
                continue;
            }
            
            char key = std::tolower(keyStr[0]);
            CommandType command = parseCommandType(commandStr);
            
            config.addBinding(key, command);
        }
        
        file.close();
        
        if (!config.validate()) {
            std::cerr << "Warning: Invalid configuration in file '" << filename 
                      << "'. Using default configuration." << std::endl;
            return InputConfig::createDefault();
        }
        
        std::cout << "Configuration loaded successfully from '" << filename << "'" << std::endl;
        return config;
    }
    
private:
    static CommandType parseCommandType(const std::string& str) {
        if (str == "MOVE_UP") return CommandType::MOVE_UP;
        if (str == "MOVE_DOWN") return CommandType::MOVE_DOWN;
        if (str == "MOVE_LEFT") return CommandType::MOVE_LEFT;
        if (str == "MOVE_RIGHT") return CommandType::MOVE_RIGHT;
        if (str == "ATTACK") return CommandType::ATTACK;
        if (str == "AREA_ATTACK") return CommandType::AREA_ATTACK;
        if (str == "SWITCH_COMBAT") return CommandType::SWITCH_COMBAT;
        if (str == "LIST_SPELLS") return CommandType::LIST_SPELLS;
        if (str == "USE_SPELL") return CommandType::USE_SPELL;
        if (str == "BUY_SPELL") return CommandType::BUY_SPELL;
        if (str == "UPGRADE_SPELL") return CommandType::UPGRADE_SPELL;
        if (str == "HELP") return CommandType::HELP;
        if (str == "SAVE") return CommandType::SAVE;
        if (str == "QUIT") return CommandType::QUIT;
        
        throw std::runtime_error("Unknown command type: " + str);
    }
};

#endif