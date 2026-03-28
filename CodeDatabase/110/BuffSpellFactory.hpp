#ifndef BUFF_SPELL_F
#define BUFF_SPELL_F

#include "../BaseSpellFactory/SpellFactory.hpp"
#include "../../../Spells/Spell/BuffSpell/BuffSpell.hpp"
#include "../../../ChangeableFields/CastChangeableFields/BuffFields/BuffFields.hpp"

class BuffSpellFactory : public SpellFactory {
    private:
    size_t level;

    public:
    BuffSpellFactory (size_t level=0);

    Spell* createSpell (ChangeableFields& changeableFields) override;
};

#endif