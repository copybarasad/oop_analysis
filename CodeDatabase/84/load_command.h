#ifndef LOAD_COMMAND_H
#define LOAD_COMMAND_H

#include "command.h"
#include <iostream>
#include <string>

class LoadCommand : public Command {
public:
    bool execute(Game& game) override {
        std::cout << "Введите имя файла для загрузки: ";
        std::string filename;
        std::getline(std::cin, filename);
        if (!filename.empty()) game.load_game(filename);
        return false;
    }
};

#endif