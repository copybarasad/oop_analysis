#ifndef QUIT_COMMAND_H
#define QUIT_COMMAND_H

#include "command.h"

class QuitCommand : public Command {
public:
    void execute(Game& game) override;
    const char* getDescription() const override { return "Quit Game"; }
};

#endif