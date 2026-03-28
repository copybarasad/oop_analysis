#ifndef DEFAULT_INPUT_STRATEGY_H
#define DEFAULT_INPUT_STRATEGY_H

#include "IInputStrategy.h"
#include "PlayerCommand.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <vector>
#include <algorithm>
#include <limits>
#include <map>

class DefaultInputStrategy : public IInputStrategy {
private:
    std::map<char, CommandType> keyBindings;
    std::map<char, MoveDirection> moveDirections;
    
    void setupDefaultBindings() {
        keyBindings.clear();
        moveDirections.clear();
        
        keyBindings['w'] = CommandType::MOVE;
        keyBindings['a'] = CommandType::MOVE;
        keyBindings['s'] = CommandType::MOVE;
        keyBindings['d'] = CommandType::MOVE;
        
        moveDirections['w'] = MoveDirection::North;
        moveDirections['a'] = MoveDirection::West;
        moveDirections['s'] = MoveDirection::South;
        moveDirections['d'] = MoveDirection::East;
        
        keyBindings['c'] = CommandType::SWITCH_COMBAT;
        keyBindings['x'] = CommandType::RANGED_ATTACK;
        keyBindings['p'] = CommandType::OPEN_SHOP;
        keyBindings['i'] = CommandType::SHOW_INFO;
        keyBindings['v'] = CommandType::SAVE_GAME;
        keyBindings['l'] = CommandType::LOAD_GAME;
        keyBindings['m'] = CommandType::MAIN_MENU;
        keyBindings['q'] = CommandType::EXIT_GAME;
    }
    
    MoveDirection charToDirection(char c) const {
        auto it = moveDirections.find(c);
        if (it != moveDirections.end()) {
            return it->second;
        }
        return MoveDirection::North;
    }
    
    CommandType stringToCommandType(const std::string& str) const {
        if (str == "MOVE") return CommandType::MOVE;
        if (str == "SWITCH_COMBAT") return CommandType::SWITCH_COMBAT;
        if (str == "RANGED_ATTACK") return CommandType::RANGED_ATTACK;
        if (str == "CAST_SPELL") return CommandType::CAST_SPELL;
        if (str == "OPEN_SHOP") return CommandType::OPEN_SHOP;
        if (str == "SHOW_INFO") return CommandType::SHOW_INFO;
        if (str == "SAVE_GAME") return CommandType::SAVE_GAME;
        if (str == "LOAD_GAME") return CommandType::LOAD_GAME;
        if (str == "MAIN_MENU") return CommandType::MAIN_MENU;
        if (str == "EXIT_GAME") return CommandType::EXIT_GAME;
        return CommandType::INVALID;
    }
    
    MoveDirection stringToMoveDirection(const std::string& str) const {
        if (str == "NORTH" || str == "north" || str == "N" || str == "n") return MoveDirection::North;
        if (str == "SOUTH" || str == "south" || str == "S" || str == "s") return MoveDirection::South;
        if (str == "WEST" || str == "west" || str == "W" || str == "w") return MoveDirection::West;
        if (str == "EAST" || str == "east" || str == "E" || str == "e") return MoveDirection::East;
        return MoveDirection::North;
    }
    
    bool validateBindings(const std::map<char, CommandType>& bindings) const {
        int moveCount = 0;
        bool hasSwitchCombat = false;
        bool hasRangedAttack = false;
        
        for (const auto& binding : bindings) {
            if (binding.second == CommandType::MOVE) moveCount++;
            if (binding.second == CommandType::SWITCH_COMBAT) hasSwitchCombat = true;
            if (binding.second == CommandType::RANGED_ATTACK) hasRangedAttack = true;
        }
        
        return moveCount >= 4 && hasSwitchCombat && hasRangedAttack;
    }
    
public:
    DefaultInputStrategy() {
        setupDefaultBindings();
    }
    
    PlayerCommand getCommand() override {
        std::cout << "Enter command: ";
        std::string input;
        std::cin >> input;
        
        PlayerCommand command;
        
        if (input.length() == 1) {
            char key = input[0];
            
            auto it = keyBindings.find(key);
            if (it != keyBindings.end()) {
                command.type = it->second;
                
                if (command.type == CommandType::MOVE) {
                    auto dirIt = moveDirections.find(key);
                    if (dirIt != moveDirections.end()) {
                        command.direction = dirIt->second;
                    } else {
                        command.type = CommandType::INVALID;
                    }
                }
            } else {
                command.type = CommandType::INVALID;
            }
        } else if (input == "shop") {
            command.type = CommandType::OPEN_SHOP;
        } else if (input == "info") {
            command.type = CommandType::SHOW_INFO;
        } else if (input == "save") {
            command.type = CommandType::SAVE_GAME;
            std::cout << "Enter save file name: ";
            std::cin >> command.filename;
        } else if (input == "load") {
            command.type = CommandType::LOAD_GAME;
            std::cout << "Enter save file name: ";
            std::cin >> command.filename;
        } else if (input == "menu") {
            command.type = CommandType::MAIN_MENU;
        } else if (input == "exit") {
            command.type = CommandType::EXIT_GAME;
        } else if (input.length() >= 2 && input[0] == 'm' && std::isdigit(input[1])) {
            command.type = CommandType::CAST_SPELL;
            command.spellIndex = input[1] - '1';
            std::cout << "Enter target coordinates (row col): ";
            std::cin >> command.target.row >> command.target.col;
        } else {
            command.type = CommandType::INVALID;
        }
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return command;
    }
    
    void showHelp() override {
        std::cout << "\n=== Controls ===\n";
        
        std::cout << "Movement:\n";
        for (const auto& dir : moveDirections) {
            std::cout << "  " << dir.first << " - ";
            switch (dir.second) {
                case MoveDirection::North: std::cout << "North/Up"; break;
                case MoveDirection::South: std::cout << "South/Down"; break;
                case MoveDirection::West: std::cout << "West/Left"; break;
                case MoveDirection::East: std::cout << "East/Right"; break;
            }
            std::cout << "\n";
        }
        
        std::cout << "\nOther commands:\n";
        for (const auto& binding : keyBindings) {
            if (binding.second != CommandType::MOVE) {
                std::cout << "  " << binding.first << " - ";
                switch (binding.second) {
                    case CommandType::SWITCH_COMBAT: std::cout << "Switch combat mode"; break;
                    case CommandType::RANGED_ATTACK: std::cout << "Ranged attack"; break;
                    case CommandType::OPEN_SHOP: std::cout << "Open shop"; break;
                    case CommandType::SHOW_INFO: std::cout << "Show info"; break;
                    case CommandType::SAVE_GAME: std::cout << "Save game"; break;
                    case CommandType::LOAD_GAME: std::cout << "Load game"; break;
                    case CommandType::MAIN_MENU: std::cout << "Main menu"; break;
                    case CommandType::EXIT_GAME: std::cout << "Exit game"; break;
                    default: break;
                }
                std::cout << "\n";
            }
        }
        
        std::cout << "\nText commands: shop, info, save, load, menu, exit\n";
        std::cout << "Spells: m1, m2, m3... (followed by target coordinates)\n";
    }
    
    void loadKeyBindingsFromFile(const std::string& filename) override {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return;
        }
        
        std::map<char, CommandType> newKeyBindings;
        std::map<char, MoveDirection> newMoveDirections;
        
        std::string line;
        int lineNumber = 0;
        bool hasErrors = false;
        
        int moveCount = 0;
        bool hasSwitchCombat = false;
        bool hasRangedAttack = false;
        
        while (std::getline(file, line)) {
            lineNumber++;
            
            if (line.empty()) continue;
            
            size_t commentPos = line.find('#');
            if (commentPos != std::string::npos) {
                line = line.substr(0, commentPos);
            }
            
            line.erase(0, line.find_first_not_of(" \t"));
            line.erase(line.find_last_not_of(" \t") + 1);
            
            if (line.empty()) continue;
            
            std::istringstream iss(line);
            std::string commandStr, keyStr;
            
            if (!(iss >> commandStr >> keyStr)) {
                hasErrors = true;
                continue;
            }
            
            if (keyStr.length() != 1) {
                hasErrors = true;
                continue;
            }
            
            char key = keyStr[0];
            CommandType cmdType = stringToCommandType(commandStr);
            
            if (cmdType == CommandType::INVALID) {
                hasErrors = true;
                continue;
            }
            
            if (newKeyBindings.find(key) != newKeyBindings.end()) {
                hasErrors = true;
                continue;
            }
            
            if (cmdType == CommandType::MOVE) {
                moveCount++;
                
                std::string directionStr;
                if (iss >> directionStr) {
                    MoveDirection dir = stringToMoveDirection(directionStr);
                    if (dir == MoveDirection::North || dir == MoveDirection::South || 
                        dir == MoveDirection::West || dir == MoveDirection::East) {
                        newMoveDirections[key] = dir;
                    } else {
                        hasErrors = true;
                        continue;
                    }
                } else {
                    hasErrors = true;
                    continue;
                }
            } else if (cmdType == CommandType::SWITCH_COMBAT) {
                hasSwitchCombat = true;
            } else if (cmdType == CommandType::RANGED_ATTACK) {
                hasRangedAttack = true;
            }
            
            newKeyBindings[key] = cmdType;
        }
        
        file.close();
        
        if (moveCount < 4) hasErrors = true;
        if (!hasSwitchCombat) hasErrors = true;
        if (!hasRangedAttack) hasErrors = true;
        
        std::vector<MoveDirection> usedDirections;
        for (const auto& dir : newMoveDirections) {
            if (std::find(usedDirections.begin(), usedDirections.end(), dir.second) != usedDirections.end()) {
                hasErrors = true;
                break;
            }
            usedDirections.push_back(dir.second);
        }
        
        if (!hasErrors) {
            keyBindings = newKeyBindings;
            moveDirections = newMoveDirections;
            std::cout << "Key bindings loaded from " << filename << "\n";
        } else {
            std::cout << "Using default bindings.\n";
        }
    }
    
    MoveDirection getMoveDirection(char key) const {
        auto it = moveDirections.find(key);
        if (it != moveDirections.end()) {
            return it->second;
        }
        return MoveDirection::North;
    }
};

#endif 