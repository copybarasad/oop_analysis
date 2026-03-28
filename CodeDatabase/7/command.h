#ifndef COMMAND_H
#define COMMAND_H

#include <string>

class Game;

class Command {
public:
    virtual ~Command() = default;
    virtual bool execute(Game& game, std::string& resultMessage) = 0;
    virtual std::string getType() const = 0;
};

#endif // COMMAND_H