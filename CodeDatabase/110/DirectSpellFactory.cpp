#include "DirectSpellFactory.hpp"

DirectSpellFactory::DirectSpellFactory (unsigned int damage, size_t range) :
    damage(damage), range(range) {}

Spell* DirectSpellFactory::createSpell (ChangeableFields& changeableFields) {
    RangeFields& rf = dynamic_cast<RangeFields&>(changeableFields);
    DirectSpell* spell = new DirectSpell(rf.getField(), damage, range, rf.getCaster(), rf.getTarget());
    spell->upgradeSpell(rf.getHand().getHandLevel());

    return spell;
}