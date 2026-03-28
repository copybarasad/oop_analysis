#ifndef COMMANDS_CAST_SPELL_COMMAND_H
#define COMMANDS_CAST_SPELL_COMMAND_H

#include "ICommand.h"

class CastSpellCommand : public ICommand {
public:
    void execute(Game& game) override;
};

#endif