#ifndef SIMPLE_INPUT_STRATEGY_H
#define SIMPLE_INPUT_STRATEGY_H

#include "IInputStrategy.h"  
#include "PlayerCommand.h"
#include <iostream>
#include <string>
#include <map>
#include <cctype>

class SimpleInputStrategy : public IInputStrategy {
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
    
public:
    SimpleInputStrategy() {
        setupDefaultBindings();
    }
    
    PlayerCommand getCommand() override {
        PlayerCommand command;
        std::string input;
        
        std::cout << "\n> ";
        if (!std::getline(std::cin, input)) {
            command.type = CommandType::EXIT_GAME;
            return command;
        }
        
        if (input.empty()) {
            command.type = CommandType::INVALID;
            return command;
        }
        
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
        } else if (input == "info" || input == "help") {
            command.type = CommandType::SHOW_INFO;
        } else if (input == "save") {
            command.type = CommandType::SAVE_GAME;
            std::cout << "Enter filename (without .sav): ";
            std::cin >> command.filename;
            std::cin.ignore();
        } else if (input == "load") {
            command.type = CommandType::LOAD_GAME;
            std::cout << "Enter filename (without .sav): ";
            std::cin >> command.filename;
            std::cin.ignore();
        } else if (input == "menu") {
            command.type = CommandType::MAIN_MENU;
        } else if (input == "exit") {
            command.type = CommandType::EXIT_GAME;
        } else if (input.length() >= 2 && input[0] == 'm' && std::isdigit(input[1])) {
            command.type = CommandType::CAST_SPELL;
            command.spellIndex = input[1] - '1';
            
            std::cout << "Enter target coordinates (row col): ";
            std::cin >> command.target.row >> command.target.col;
            std::cin.ignore();
        } else {
            command.type = CommandType::INVALID;
        }
        
        return command;
    }
    
    void showHelp() override {
        std::cout << "\n=== CONTROLS ===\n"
                  << "Movement:\n"
                  << "  w - Move North/Up\n"
                  << "  a - Move West/Left\n"
                  << "  s - Move South/Down\n"
                  << "  d - Move East/Right\n"
                  << "\nActions:\n"
                  << "  c - Switch combat style (Melee/Ranged)\n"
                  << "  x - Ranged attack (ranged mode only)\n"
                  << "  m1, m2, m3... - Cast spell from slot\n"
                  << "\nOther:\n"
                  << "  shop - Open spell shop\n"
                  << "  info/help - Show this help\n"
                  << "  save - Save game\n"
                  << "  load - Load game\n"
                  << "  menu - Return to main menu\n"
                  << "  exit - Exit game\n";
    }
    
    void loadKeyBindingsFromFile(const std::string& filename) override {
        std::cout << "Using default key bindings\n";
    }
};

#endif 