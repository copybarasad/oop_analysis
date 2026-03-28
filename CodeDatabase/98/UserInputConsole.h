#ifndef USER_INPUT_CONSOLE_H
#define USER_INPUT_CONSOLE_H

#include "CommandManager.h"
#include <iostream>
#include <limits>
#include <string>

class UserInputConsole {
public:
    Command GetCommand() {
        std::string inputLine;
        std::getline(std::cin, inputLine);
        
        if (inputLine.empty()) {
            return Command(CommandType::None);
        }
        
        char firstChar = inputLine[0];
        
        // Если команда 'v' и есть параметр
        if (firstChar == 'v' && inputLine.length() > 1) {
            try {
                std::string paramStr = inputLine.substr(1);
                paramStr.erase(0, paramStr.find_first_not_of(" \t"));
                int param = std::stoi(paramStr);
                return Command(CommandType::UseSpell, param);
            } catch (...) {
                return Command(CommandType::None);
            }
        }
        
        // остальны5
        switch (firstChar) {
            case 'w': return Command(CommandType::MoveUp);
            case 's': return Command(CommandType::MoveDown);
            case 'a': return Command(CommandType::MoveLeft);
            case 'd': return Command(CommandType::MoveRight);
            case 'f': return Command(CommandType::Attack);
            case 'p': return Command(CommandType::PrintField);
            case 'm': return Command(CommandType::ShowStats);
            case 'q': return Command(CommandType::Quit);
            case 'r': return Command(CommandType::SwitchAttackType);
            case 'e': return Command(CommandType::SearchEnemies);
            case 'S': return Command(CommandType::Save);
            case 'L': return Command(CommandType::Load);
            default:
                return Command(CommandType::None);
        }
    }
    
    std::pair<int, int> ReadCoordinates() {
        int x, y;
        std::cout << "Введите координаты X Y: ";
        if (std::cin >> x >> y) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return {x, y};
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return {-1, -1};
        }
    }
};

#endif