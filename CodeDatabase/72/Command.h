//
// Created by Mac on 02.12.2025.
//

#ifndef GAME_TERM_COMMAND_H
#define GAME_TERM_COMMAND_H

#include "CommandResult.h"
#include "CommandType.h"
#include "../Game.h"

class Game;

class Command {
public:
    virtual ~Command() = default;
    virtual CommandType getType() const = 0;
    virtual CommandResult execute(Game& game) = 0;
};

#endif //GAME_TERM_COMMAND_H