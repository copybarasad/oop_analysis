#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <memory>

class GameState;

class Command {
public:
    virtual ~Command() = default;
    virtual bool execute(GameState& gameState) = 0;
    virtual std::string getName() const = 0;
};

#endif