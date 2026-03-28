#ifndef SWITCH_MODE_COMMAND_H
#define SWITCH_MODE_COMMAND_H

#include "command.h"

class SwitchModeCommand : public Command {
public:
    void execute(Game& game) override;
    const char* getDescription() const override { return "Switch Combat Mode"; }
};

#endif