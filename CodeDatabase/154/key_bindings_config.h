#ifndef KEY_BINDINGS_CONFIG_H
#define KEY_BINDINGS_CONFIG_H

#include "command.h"
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

class KeyBindingsConfig {
private:
    static inline const std::map<std::string, CommandType> COMMAND_MAP = {
        {"MoveUp", CommandType::MoveUp},
        {"MoveDown", CommandType::MoveDown},
        {"MoveLeft", CommandType::MoveLeft},
        {"MoveRight", CommandType::MoveRight},
        {"Attack", CommandType::Attack},
        {"CastSpell", CommandType::CastSpell},
        {"ShowInfo", CommandType::ShowInfo},
        {"SaveGame", CommandType::SaveGame},
        {"LoadGame", CommandType::LoadGame},
        {"Quit", CommandType::Quit}
    };

    static std::string getCommandName(CommandType cmd) {
        for (const auto& pair : COMMAND_MAP) {
            if (pair.second == cmd) {
                return pair.first;
            }
        }
        return "unknown";
    }

public:
    
    static bool loadConfigFile(const std::string& filename, 
                              std::map<char, CommandType>& bindings) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "[WARNING] Config file not found: " << filename 
                     << ". Using default key bindings.\n";
            return false;
        }

        std::map<char, CommandType> tempBindings;
        std::map<CommandType, char> reverseMap;
        std::string line;
        bool hasErrors = false;

        while (std::getline(file, line)) {
            
            if (line.empty() || line[0] == '#') continue;

            std::stringstream ss(line);
            std::string commandName;
            char key;

            ss >> commandName >> key;

            if (COMMAND_MAP.find(commandName) == COMMAND_MAP.end()) {
                std::cerr << "[WARNING] Unknown command in config: " << commandName << "\n";
                hasErrors = true;
                continue;
            }

            CommandType cmd = COMMAND_MAP.at(commandName);

            if (tempBindings.count(key) && tempBindings[key] != cmd) {
                std::cerr << "[WARNING] Key '" << key << "' mapped to multiple commands\n";
                hasErrors = true;
                continue;
            }

            if (reverseMap.count(cmd) && reverseMap[cmd] != key) {
                std::cerr << "[WARNING] Command '" << commandName << "' mapped to multiple keys\n";
                hasErrors = true;
                continue;
            }

            tempBindings[key] = cmd;
            reverseMap[cmd] = key;
        }

        file.close();

        std::vector<CommandType> requiredCommands = {
            CommandType::MoveUp, CommandType::MoveDown, CommandType::MoveLeft, CommandType::MoveRight,
            CommandType::Attack, CommandType::SaveGame, CommandType::Quit
        };

        for (CommandType cmd : requiredCommands) {
            if (reverseMap.find(cmd) == reverseMap.end()) {
                std::cerr << "[WARNING] Missing required command: " << getCommandName(cmd) << "\n";
                hasErrors = true;
            }
        }

        if (hasErrors) {
            std::cerr << "[WARNING] Config file has errors. Using default key bindings.\n";
            return false;
        }

        bindings = tempBindings;
        return true;
    }

    static bool createDefaultConfig(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "[ERROR] Could not create config file: " << filename << "\n";
            return false;
        }

        file << "# Game Key Bindings Configuration\n";
        file << "# Format: COMMAND_NAME KEY\n";
        file << "# Example: MoveUp w\n\n";
        file << "MoveUp w\n";
        file << "MoveDown s\n";
        file << "MoveLeft a\n";
        file << "MoveRight d\n";
        file << "Attack e\n";
        file << "CastSpell c\n";
        file << "ShowInfo q\n";
        file << "SaveGame p\n";
        file << "LoadGame l\n";
        file << "Quit x\n";

        file.close();
        return true;
    }
};

#endif
