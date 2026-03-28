#ifndef COMMAND_H
#define COMMAND_H

#include "game.h"

class Command {
public:
    virtual ~Command() = default;
    virtual bool execute(Game& game) = 0;
};

#endif