#ifndef MOVE_DOWN_COMMAND_H
#define MOVE_DOWN_COMMAND_H

#include "command.h"

class MoveDownCommand : public Command {
public:
    void execute(Game& game) override;
    const char* getDescription() const override { return "Move Down"; }
};

#endif