#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H

#include "command.h"

class MoveCommand : public Command {
    int dx_, dy_;
public:
    MoveCommand(int dx, int dy) : dx_(dx), dy_(dy) {}
    bool execute(Game& game) override {
        return game.move_player(dx_, dy_);
    }
};

#endif