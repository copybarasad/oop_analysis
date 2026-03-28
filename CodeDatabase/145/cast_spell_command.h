#ifndef CAST_SPELL_COMMAND_H
#define CAST_SPELL_COMMAND_H

#include "command.h"

class CastSpellCommand : public Command {
private:
    int spellIndex;

public:
    explicit CastSpellCommand(int index) : spellIndex(index) {}
    void execute(Game& game) override;
    const char* getDescription() const override { return "Cast Spell"; }
};

#endif