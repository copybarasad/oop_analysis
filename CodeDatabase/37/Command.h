#ifndef COMMAND_H
#define COMMAND_H

#include <string>

class Game;

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(Game& game) = 0;
    virtual std::string getName() const = 0;
    virtual Command* clone() const = 0;
};

#endif