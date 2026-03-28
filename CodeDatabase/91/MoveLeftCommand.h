#ifndef COMMANDS_MOVE_LEFT_COMMAND_H
#define COMMANDS_MOVE_LEFT_COMMAND_H

#include "ICommand.h"

class MoveLeftCommand : public ICommand {
public:
    void execute(Game& game) override;
};

#endif