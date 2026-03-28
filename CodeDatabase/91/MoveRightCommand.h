#ifndef COMMANDS_MOVE_RIGHT_COMMAND_H
#define COMMANDS_MOVE_RIGHT_COMMAND_H

#include "ICommand.h"

class MoveRightCommand : public ICommand {
public:
    void execute(Game& game) override;
};

#endif