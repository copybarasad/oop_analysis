#ifndef SKIP_TURN_COMMAND_H
#define SKIP_TURN_COMMAND_H

#include "Command.h"

class SkipTurnCommand : public Command {
public:
    CommandResult execute(Game& game) override;
    bool EndsTurn() const override { return true; }
};

#endif
