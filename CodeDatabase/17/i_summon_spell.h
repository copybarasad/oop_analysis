#ifndef I_SUMMON_SPELL_H
#define I_SUMMON_SPELL_H

#include "ispell.h"

class ISummonSpell : public ISpell {
public:
    virtual bool cast(GameMap& map, Position casterPos, Position targetPos) = 0;
    virtual std::string getName() const = 0;
    virtual int getRange(int lev) const = 0;
};

#endif