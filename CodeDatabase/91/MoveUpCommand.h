#ifndef COMMANDS_MOVE_UP_COMMAND_H
#define COMMANDS_MOVE_UP_COMMAND_H

#include "ICommand.h"

class MoveUpCommand : public ICommand {
public:
    void execute(Game& game) override;
};

#endif