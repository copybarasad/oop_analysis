#ifndef ALLY_SPELL_F
#define ALLY_SPELL_F

#include "../BaseSpellFactory/SpellFactory.hpp"
#include "../../../Spells/Spell/RangedSpell/SpawnSpell/AllySpell/AllySpell.hpp"
#include "../../../ChangeableFields/CastChangeableFields/RangeFields/RangeFields.hpp"

class AllySpellFactory : public SpellFactory {
    private:
    size_t range;

    public:
    AllySpellFactory (size_t range);

    Spell* createSpell (ChangeableFields& changeableFields) override;
};

#endif