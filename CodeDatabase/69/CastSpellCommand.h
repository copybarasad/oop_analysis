#ifndef CAST_SPELL_COMMAND_H
#define CAST_SPELL_COMMAND_H

#include "Command.h"

class CastSpellCommand : public Command {
public:
    CastSpellCommand(int index, int x, int y);
    CommandResult execute(Game& game) override;
    bool EndsTurn() const override { return true; }
private:
    int index_;
    int x_;
    int y_;
};

#endif
