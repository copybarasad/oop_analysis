#ifndef CAST_SPELL_COMMAND_H
#define CAST_SPELL_COMMAND_H

#include "command_interface.h"
#include "position.h"

class CastSpellCommand : public ICommand {
private:
    int spellIndex;
    Position target;
    
public:
    CastSpellCommand(int index, const Position& pos);
    bool execute(GameController& game) override;
    std::string getDescription() const override;
    bool isValid() const override;
    CommandType getCommandType() const override { return CommandType::CastSpell; }
};

#endif