#pragma once
#include <string>

class Game;

class Command {
public:
    virtual ~Command() = default;
    virtual bool execute(Game& game) = 0;
    virtual std::string getName() const = 0;
};
