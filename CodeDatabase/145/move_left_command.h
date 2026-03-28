#ifndef MOVE_LEFT_COMMAND_H
#define MOVE_LEFT_COMMAND_H

#include "command.h"

class MoveLeftCommand : public Command {
public:
    void execute(Game& game) override;
    const char* getDescription() const override { return "Move Left"; }
};

#endif