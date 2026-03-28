#pragma once

#include <memory>
#include <unistd.h>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <functional>
#include <vector>

#include "commands/ICommand.h"
#include "commands/MoveCommand.h"
#include "commands/AttackCommand.h"
#include "commands/ChangeAttackModeCommand.h"
#include "commands/QuitCommand.h"
#include "commands/ConfirmCommand.h"
#include "commands/CancelCommand.h"
#include "commands/CastSpellCommand.h"
#include "commands/SaveGameCommand.h"
#include "commands/LoadGameCommand.h"

class ConsoleInputReader {
private:
    std::map<char, std::function<std::unique_ptr<ICommand>()>> keyBindings;
    
    void loadDefaultBindings() {
        keyBindings['w'] = []() { return std::make_unique<MoveCommand>(Direction::UP); };
        keyBindings['a'] = []() { return std::make_unique<MoveCommand>(Direction::LEFT); };
        keyBindings['s'] = []() { return std::make_unique<MoveCommand>(Direction::DOWN); };
        keyBindings['d'] = []() { return std::make_unique<MoveCommand>(Direction::RIGHT); };
        keyBindings[' '] = []() { return std::make_unique<AttackCommand>(); };
        keyBindings['e'] = []() { return std::make_unique<ChangeAttackModeCommand>(); };
        keyBindings['q'] = []() { return std::make_unique<QuitCommand>(); };
        keyBindings['\n'] = []() { return std::make_unique<ConfirmCommand>(); };
        keyBindings[27] = []() { return std::make_unique<CancelCommand>(); }; // ESC
        keyBindings['c'] = []() { return std::make_unique<CastSpellCommand>(); };
        keyBindings['f'] = []() { return std::make_unique<SaveGameCommand>(); };
        keyBindings['l'] = []() { return std::make_unique<LoadGameCommand>(); };
    }

    bool validateBindings(const std::map<std::string, char>& config) {
        std::map<char, int> keyCount;
        for (const auto& [cmd, key] : config) {
            keyCount[key]++;
            if (keyCount[key] > 1) {
                return false;
            }
        }

        std::map<std::string, int> cmdCount;
        for (const auto& [cmd, key] : config) {
            cmdCount[cmd]++;
            if (cmdCount[cmd] > 1) {
                return false;
            }
        }

        std::vector<std::string> requiredCommands = {
            "move_up", "move_down", "move_left", "move_right",
            "attack", "change_mode", "quit", "confirm", "cancel",
            "cast_spell", "save", "load"
        };

        for (const auto& cmd : requiredCommands) {
            if (config.find(cmd) == config.end()) {
                return false;
            }
        }

        return true;
    }

    void applyBindings(const std::map<std::string, char>& config) {
        keyBindings.clear();
        
        keyBindings[config.at("move_up")] = []() { 
            return std::make_unique<MoveCommand>(Direction::UP); 
        };
        keyBindings[config.at("move_down")] = []() { 
            return std::make_unique<MoveCommand>(Direction::DOWN); 
        };
        keyBindings[config.at("move_left")] = []() { 
            return std::make_unique<MoveCommand>(Direction::LEFT); 
        };
        keyBindings[config.at("move_right")] = []() { 
            return std::make_unique<MoveCommand>(Direction::RIGHT); 
        };
        keyBindings[config.at("attack")] = []() { 
            return std::make_unique<AttackCommand>(); 
        };
        keyBindings[config.at("change_mode")] = []() { 
            return std::make_unique<ChangeAttackModeCommand>(); 
        };
        keyBindings[config.at("quit")] = []() { 
            return std::make_unique<QuitCommand>(); 
        };
        keyBindings[config.at("confirm")] = []() { 
            return std::make_unique<ConfirmCommand>(); 
        };
        keyBindings[config.at("cancel")] = []() { 
            return std::make_unique<CancelCommand>(); 
        };
        keyBindings[config.at("cast_spell")] = []() { 
            return std::make_unique<CastSpellCommand>(); 
        };
        keyBindings[config.at("save")] = []() { 
            return std::make_unique<SaveGameCommand>(); 
        };
        keyBindings[config.at("load")] = []() { 
            return std::make_unique<LoadGameCommand>(); 
        };
    }

public:
    ConsoleInputReader() {
        loadDefaultBindings();
    }

    void loadKeybindings(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            loadDefaultBindings();
            return;
        }

        std::map<std::string, char> config;
        std::string line;

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;

            std::istringstream iss(line);
            std::string command, keyStr;

            if (!(iss >> command)) continue;
            
            std::string rest;
            std::getline(iss, rest);
            
            size_t start = rest.find_first_not_of(" \t");
            if (start != std::string::npos) {
                keyStr = rest.substr(start);
            }
            
            char key;
            if (keyStr.empty() || keyStr == "space") {
                key = ' ';
            } else if (keyStr == "enter" || keyStr == "newline") {
                key = '\n';
            } else if (keyStr == "esc" || keyStr == "escape") {
                key = 27;
            } else if (keyStr.length() > 0) {
                key = keyStr[0];
            } else {
                continue;
            }
            
            config[command] = key;
        }

        file.close();

        if (validateBindings(config)) {
            applyBindings(config);
        } else {
            loadDefaultBindings();
        }
    }

    char readKey() {
        char ch = ' ';
        read(STDIN_FILENO, &ch, 1);
        return ch;
    }

    std::unique_ptr<ICommand> getCommand() {
        char key = readKey();
        
        auto it = keyBindings.find(key);
        if (it != keyBindings.end()) {
            return it->second();
        }
        
        return nullptr;
    }

    char getChar() {
        return readKey();
    }
};
