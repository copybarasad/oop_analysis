#ifndef BUY_SPELL_COMMAND_H
#define BUY_SPELL_COMMAND_H

#include "Command.h"

class BuySpellCommand : public Command {
public:
    CommandResult execute(Game& game) override;
    bool EndsTurn() const override { return true; }
};

#endif
