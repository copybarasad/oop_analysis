#ifndef MOVE_UP_COMMAND_H
#define MOVE_UP_COMMAND_H

#include "command.h"

class MoveUpCommand : public Command {
public:
    void execute(Game& game) override;
    const char* getDescription() const override { return "Move Up"; }
};

#endif