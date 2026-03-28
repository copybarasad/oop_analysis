#ifndef SPELL_F
#define SPELL_F

#include "../../../Spells/Spell/BaseSpell/Spell.hpp"
#include "../../../ChangeableFields/BaseChangeableFields/ChangebaleFields.hpp"

class SpellFactory {
    public:
    virtual ~SpellFactory () = default;
    virtual Spell* createSpell (ChangeableFields& changeableFields) = 0;
};

#endif