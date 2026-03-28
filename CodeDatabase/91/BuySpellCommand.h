#ifndef COMMANDS_BUY_SPELL_COMMAND_H
#define COMMANDS_BUY_SPELL_COMMAND_H

#include "ICommand.h"

class BuySpellCommand : public ICommand {
public:
    void execute(Game& game) override;
};

#endif