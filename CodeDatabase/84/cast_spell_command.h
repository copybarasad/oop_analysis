#ifndef CAST_SPELL_COMMAND_H
#define CAST_SPELL_COMMAND_H

#include "command.h"

class CastSpellCommand : public Command {
public:
    bool execute(Game& game) override {
        return game.handle_spell_cast_input();
    }
};

#endif