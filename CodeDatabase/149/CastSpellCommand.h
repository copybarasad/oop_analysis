#ifndef CASTSPELLCOMMAND_H
#define CASTSPELLCOMMAND_H

#include "Command.h"
#include "Position.h"

class CastSpellCommand : public Command {
private:
    int spellIndex;
    Position targetPosition;

public:
    CastSpellCommand(int index, const Position& target);
    CommandType getType() const override;
    bool isValid() const override;
    int getSpellIndex() const;
    Position getTargetPosition() const;
};

#endif