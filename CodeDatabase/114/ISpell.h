#ifndef SPELL_H
#define SPELL_H

#include <cmath>
#include "interfaces/ISpellField.h"

class Entity;

enum class SpellType {
    DIRECT_DAMAGE,
    AREA_DAMAGE,
    TRAP,
    IMPROVE,
    SUMMON
};

class ISpell {
public:
    virtual bool cast(int pendingBoost, Entity* target, ISpellField* field) = 0;
    virtual bool getHasTarget() = 0;
};

#endif
