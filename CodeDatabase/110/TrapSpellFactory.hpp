#ifndef TRAP_SPELL_F
#define TRAP_SPELL_F

#include "../BaseSpellFactory/SpellFactory.hpp"
#include "../../../Spells/Spell/RangedSpell/SpawnSpell/TrapSpell/TrapSpell.hpp"
#include "../../../ChangeableFields/CastChangeableFields/RangeFields/RangeFields.hpp"

class TrapSpellFactory : public SpellFactory {
    private:
    size_t range;
    
    public:
    TrapSpellFactory (size_t range);

    Spell* createSpell (ChangeableFields& changeableFields) override;
};

#endif