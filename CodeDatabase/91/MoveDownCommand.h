#ifndef COMMANDS_MOVE_DOWN_COMMAND_H
#define COMMANDS_MOVE_DOWN_COMMAND_H

#include "ICommand.h"

class MoveDownCommand : public ICommand {
public:
    void execute(Game& game) override;
};

#endif