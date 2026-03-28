#pragma once
#include "Command.h"
#include <optional>
#include <iostream>
#include <limits>
#include <cctype>


class Game;

class KeyboardInput {
public:
    std::optional<Command> getNextCommand();
    std::optional<Command> parseSpell();
private:
    bool safeInputInt(int& out);
    bool safeInputChar(char& out);
    std::optional<Command> parseHarpoon();
};