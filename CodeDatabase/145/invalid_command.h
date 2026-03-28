#ifndef INVALID_COMMAND_H
#define INVALID_COMMAND_H

#include "command.h"

class InvalidCommand : public Command {
public:
    void execute(Game& game) override;
    const char* getDescription() const override { return "Invalid Command"; }
};

#endif