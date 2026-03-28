#ifndef COMMANDS_SWITCH_COMBAT_COMMAND_H
#define COMMANDS_SWITCH_COMBAT_COMMAND_H

#include "ICommand.h"

class SwitchCombatCommand : public ICommand {
public:
    void execute(Game& game) override;
};

#endif