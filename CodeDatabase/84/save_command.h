#ifndef SAVE_COMMAND_H
#define SAVE_COMMAND_H

#include "command.h"
#include <iostream>
#include <string>

class SaveCommand : public Command {
public:
    bool execute(Game& game) override {
        std::cout << "Введите имя файла для сохранения: ";
        std::string filename;
        std::getline(std::cin, filename);
        if (!filename.empty()) game.save_game(filename);
        return false;
    }
};

#endif