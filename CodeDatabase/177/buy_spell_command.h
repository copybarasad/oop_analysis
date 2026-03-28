#ifndef BUY_SPELL_COMMAND_H
#define BUY_SPELL_COMMAND_H

#include "command_interface.h"

class BuySpellCommand : public ICommand {
public:
    bool execute(GameController& game) override;
    std::string getDescription() const override;
    bool isValid() const override;
    CommandType getCommandType() const override { return CommandType::BuySpell; }
};

#endif