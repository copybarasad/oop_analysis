#pragma once

#include "command.h"

#include <istream>

class Game;

class ConsoleInput {
public:
    ConsoleInput();
    explicit ConsoleInput(std::istream& input);

    Command ReadCommand(const Game& game);

private:
    std::istream* in;
};
