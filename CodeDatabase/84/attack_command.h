#ifndef ATTACK_COMMAND_H
#define ATTACK_COMMAND_H

#include "command.h"
#include <iostream>
#include <string>

class AttackCommand : public Command {
public:
    bool execute(Game& game) override {
        std::cout << "Выберите направление атаки (w/a/s/d): ";
        std::string line;
        if (!std::getline(std::cin, line) || line.empty()) return false;
        
        int dx = 0, dy = 0;
        char dir = std::tolower(line[0]);
        if (dir == 'w') dy = -1;
        else if (dir == 's') dy = 1;
        else if (dir == 'a') dx = -1;
        else if (dir == 'd') dx = 1;
        else {
            std::cout << "Неверное направление!" << std::endl;
            return false;
        }
        
        game.player_attack(dx, dy);
        return true; 
    }
};

#endif