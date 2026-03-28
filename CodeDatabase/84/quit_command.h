#ifndef QUIT_COMMAND_H
#define QUIT_COMMAND_H

#include "command.h"

class QuitCommand : public Command {
public:
    bool execute(Game& game) override {
        game.quit();
        return false;
    }
};

#endif