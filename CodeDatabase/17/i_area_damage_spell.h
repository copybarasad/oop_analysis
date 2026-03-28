
#ifndef I_AREA_DAMAGE_SPELL_H
#define I_AREA_DAMAGE_SPELL_H

#include "ispell.h"

class IAreaDamageSpell : public ISpell {
public:
    virtual bool cast(GameMap& map, Position casterPos, Position target) = 0;
    virtual std::string getName() const = 0;
    virtual int getRange(int lev) const = 0;
};

#endif