#ifndef MOVE_RIGHT_COMMAND_H
#define MOVE_RIGHT_COMMAND_H

#include "command.h"

class MoveRightCommand : public Command {
public:
    void execute(Game& game) override;
    const char* getDescription() const override { return "Move Right"; }
};

#endif