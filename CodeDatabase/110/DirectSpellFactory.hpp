#ifndef DIRECT_SPELL_F
#define DIRECT_SPELL_F

#include "../BaseSpellFactory/SpellFactory.hpp"
#include "../../../Spells/Spell/RangedSpell/DamageSpell/DirectSpell/DirectSpell.hpp"
#include "../../../ChangeableFields/CastChangeableFields/RangeFields/RangeFields.hpp"

class DirectSpellFactory : public SpellFactory {
    private:
    unsigned int damage;
    size_t range;

    public:
    DirectSpellFactory (unsigned int damage, size_t range);

    Spell* createSpell (ChangeableFields& changeableFields) override;
};

#endif