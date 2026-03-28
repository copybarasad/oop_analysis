#ifndef CONSOLE_INPUT_HANDLER_H
#define CONSOLE_INPUT_HANDLER_H
#include "game_command.h"
#include <iostream>
#include <string>
#include <cctype>
#include <limits>
#include <map>
#include <fstream>
#include <vector>

class ConsoleInputHandler {
private:
    std::map<char, GameCommand::Type> keyBindings;
    
    std::map<char, GameCommand::Type> spellBindings;
    
    void setDefaultBindings() {
        keyBindings = {
            {'w', GameCommand::Type::MOVE_UP},
            {'s', GameCommand::Type::MOVE_DOWN},
            {'a', GameCommand::Type::MOVE_LEFT},
            {'d', GameCommand::Type::MOVE_RIGHT},
            {'c', GameCommand::Type::CAST_SPELL},
            {'t', GameCommand::Type::TOGGLE_COMBAT},
            {'v', GameCommand::Type::SAVE_GAME},
            {'q', GameCommand::Type::QUIT},
            {'e', GameCommand::Type::CONFIRM}
        };
        
        for (int i = 0; i <= 9; i++) {
            spellBindings['0' + i] = static_cast<GameCommand::Type>(
                static_cast<int>(GameCommand::Type::SPELL_0) + i
            );
        }
    }
    
    bool loadBindingsFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        
        std::map<char, GameCommand::Type> newKeyBindings;
        std::map<char, GameCommand::Type> newSpellBindings;
        std::map<GameCommand::Type, bool> commandUsed;
        
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            
            size_t eqPos = line.find('=');
            if (eqPos == std::string::npos) continue;
            
            std::string commandStr = line.substr(0, eqPos);
            std::string keyStr = line.substr(eqPos + 1);
            
            if (keyStr.empty() || commandStr.empty()) continue;
            
            char key = std::tolower(keyStr[0]);
            GameCommand::Type command = stringToCommand(commandStr);
            
            if (command == GameCommand::Type::UNKNOWN) continue;
            
            bool isSpellCommand = (command >= GameCommand::Type::SPELL_0 && 
                                   command <= GameCommand::Type::SPELL_9);
            
            if (isSpellCommand) {
                if (newSpellBindings.find(key) != newSpellBindings.end()) {
                    file.close();
                    return false;
                }
                newSpellBindings[key] = command;
            } else {
                if (newKeyBindings.find(key) != newKeyBindings.end() || 
                    commandUsed.find(command) != commandUsed.end()) {
                    file.close();
                    return false; 
                }
                newKeyBindings[key] = command;
                commandUsed[command] = true;
            }
        }
        
        file.close();
        
        std::vector<GameCommand::Type> requiredCommands = {
            GameCommand::Type::MOVE_UP, GameCommand::Type::MOVE_DOWN,
            GameCommand::Type::MOVE_LEFT, GameCommand::Type::MOVE_RIGHT,
            GameCommand::Type::CAST_SPELL, GameCommand::Type::TOGGLE_COMBAT,
            GameCommand::Type::SAVE_GAME, GameCommand::Type::QUIT
        };
        
        for (auto cmd : requiredCommands) {
            bool found = false;
            for (auto& pair : newKeyBindings) {
                if (pair.second == cmd) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false; 
            }
        }
        
        keyBindings = newKeyBindings;
        spellBindings = newSpellBindings;
        return true;
    }
    
    GameCommand::Type stringToCommand(const std::string& str) {
        if (str == "MOVE_UP") return GameCommand::Type::MOVE_UP;
        if (str == "MOVE_DOWN") return GameCommand::Type::MOVE_DOWN;
        if (str == "MOVE_LEFT") return GameCommand::Type::MOVE_LEFT;
        if (str == "MOVE_RIGHT") return GameCommand::Type::MOVE_RIGHT;
        if (str == "CAST_SPELL") return GameCommand::Type::CAST_SPELL;
        if (str == "TOGGLE_COMBAT") return GameCommand::Type::TOGGLE_COMBAT;
        if (str == "SAVE_GAME") return GameCommand::Type::SAVE_GAME;
        if (str == "QUIT") return GameCommand::Type::QUIT;
        if (str == "CONFIRM") return GameCommand::Type::CONFIRM;
        if (str == "CANCEL") return GameCommand::Type::CANCEL;
        
        if (str == "SPELL_0") return GameCommand::Type::SPELL_0;
        if (str == "SPELL_1") return GameCommand::Type::SPELL_1;
        if (str == "SPELL_2") return GameCommand::Type::SPELL_2;
        if (str == "SPELL_3") return GameCommand::Type::SPELL_3;
        if (str == "SPELL_4") return GameCommand::Type::SPELL_4;
        if (str == "SPELL_5") return GameCommand::Type::SPELL_5;
        if (str == "SPELL_6") return GameCommand::Type::SPELL_6;
        if (str == "SPELL_7") return GameCommand::Type::SPELL_7;
        if (str == "SPELL_8") return GameCommand::Type::SPELL_8;
        if (str == "SPELL_9") return GameCommand::Type::SPELL_9;
        
        return GameCommand::Type::UNKNOWN;
    }
    
    char readChar() {
        std::string line;
        std::getline(std::cin, line);
        if (line.empty()) return '\0';
        return std::tolower(line[0]);
    }
    
public:
    ConsoleInputHandler() {
        if (!loadBindingsFromFile("controls.cfg")) {
            setDefaultBindings();
        }
    }
    
    ~ConsoleInputHandler() = default;

    const std::map<char, GameCommand::Type>& getKeyBindings() const { return keyBindings; }
    const std::map<char, GameCommand::Type>& getSpellBindings() const { return spellBindings; }
    
    char getKeyForCommand(GameCommand::Type command) const {
        for (const auto& pair : keyBindings) {
            if (pair.second == command) {
                return pair.first;
            }
        }
        return '\0';
    }
    
    int getSpellNumberForKey(char key) const {
        auto it = spellBindings.find(key);
        if (it != spellBindings.end()) {
            return static_cast<int>(it->second) - static_cast<int>(GameCommand::Type::SPELL_0);
        }
        return -1;
    }
    
    GameCommand getGameCommand() {
        char input = readChar();
        auto it = keyBindings.find(input);
        if (it != keyBindings.end()) {
            return GameCommand(it->second);
        }
        return GameCommand(GameCommand::Type::UNKNOWN);
    }
    
    GameCommand getSpellCommand() {
        char input = readChar();
        
        auto spellIt = spellBindings.find(input);
        if (spellIt != spellBindings.end()) {
            return GameCommand(spellIt->second, input);
        }
        
        if (input >= '0' && input <= '9') {
            return GameCommand(static_cast<GameCommand::Type>(
                static_cast<int>(GameCommand::Type::SPELL_0) + (input - '0')
            ), input);
        }
        
        auto it = keyBindings.find(input);
        if (it != keyBindings.end() && it->second == GameCommand::Type::QUIT) {
            return GameCommand(GameCommand::Type::QUIT);
        }
        
        return GameCommand(GameCommand::Type::UNKNOWN);
    }
    
    GameCommand getTargetCommand() {
        char input = readChar();
        auto it = keyBindings.find(input);
        if (it != keyBindings.end()) {
            if (it->second == GameCommand::Type::CONFIRM) {
                return GameCommand(GameCommand::Type::CONFIRM);
            }
            if (it->second == GameCommand::Type::QUIT) {
                return GameCommand(GameCommand::Type::CANCEL);
            }
            
            switch(it->second) {
                case GameCommand::Type::MOVE_UP:
                case GameCommand::Type::MOVE_DOWN:
                case GameCommand::Type::MOVE_LEFT:
                case GameCommand::Type::MOVE_RIGHT:
                    return GameCommand(it->second);
                default:
                    break;
            }
        }
        
        if (input == 'e') return GameCommand(GameCommand::Type::CONFIRM);
        if (input == 'q') return GameCommand(GameCommand::Type::CANCEL);
        
        return GameCommand(GameCommand::Type::UNKNOWN);
    }
    
    GameCommand getMenuCommand() {
        char input = readChar();
        
        if (input == '1') return GameCommand(GameCommand::Type::MENU_SELECT_1);
        if (input == '2') return GameCommand(GameCommand::Type::MENU_SELECT_2);
        if (input == '3') return GameCommand(GameCommand::Type::MENU_SELECT_3);
        
        auto it = keyBindings.find(input);
        if (it != keyBindings.end() && it->second == GameCommand::Type::QUIT) {
            return GameCommand(GameCommand::Type::QUIT);
        }
        
        return GameCommand(GameCommand::Type::UNKNOWN);
    }
    
    void waitForEnter() {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
};

#endif