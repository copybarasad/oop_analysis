#ifndef QUIT_COMMAND_H
#define QUIT_COMMAND_H

#include "Command.h"

class QuitCommand : public Command {
public:
    CommandResult execute(Game& game) override;
};

#endif
