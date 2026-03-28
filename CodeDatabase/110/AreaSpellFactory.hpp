#ifndef AREA_SPELL_F
#define AREA_SPELL_F

#include "../BaseSpellFactory/SpellFactory.hpp"
#include "../../../Spells/Spell/RangedSpell/DamageSpell/AreaSpell/AreaSpell.hpp"
#include "../../../ChangeableFields/CastChangeableFields/RangeFields/RangeFields.hpp"

class AreaSpellFactory : public SpellFactory {
    private:
    unsigned int damage;
    size_t range;

    public:
    AreaSpellFactory (unsigned int damage, size_t range);

    Spell* createSpell (ChangeableFields& changeableFields) override;
};

#endif