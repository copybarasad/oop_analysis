#include "TrapSpellFactory.hpp"

TrapSpellFactory::TrapSpellFactory (size_t range) :
    range(range) {}

Spell* TrapSpellFactory::createSpell (ChangeableFields& changeableFields) {
    RangeFields& rf = dynamic_cast<RangeFields&>(changeableFields);
    TrapSpell* spell = new TrapSpell(rf.getField(), {15, 0}, range, rf.getCaster(), rf.getTarget());
    spell->upgradeSpell(rf.getHand().getHandLevel());

    return spell;
}