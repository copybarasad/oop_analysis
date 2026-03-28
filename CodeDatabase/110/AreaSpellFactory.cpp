#include "AreaSpellFactory.hpp"

AreaSpellFactory::AreaSpellFactory (unsigned int damage, size_t range) :
    damage(damage), range(range) {}

Spell* AreaSpellFactory::createSpell (ChangeableFields& changeableFields) {
    RangeFields& rf = dynamic_cast<RangeFields&>(changeableFields);
    AreaSpell* spell = new AreaSpell(rf.getField(), damage, range, rf.getCaster(), rf.getTarget());
    spell->upgradeSpell(rf.getHand().getHandLevel());

    return spell;
}