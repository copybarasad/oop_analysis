#ifndef BUY_SPELL_COMMAND_H
#define BUY_SPELL_COMMAND_H

#include "command.h"

class BuySpellCommand : public Command {
public:
    bool execute(Game& game) override {
        return game.buy_spell_interaction();
    }
};

#endif