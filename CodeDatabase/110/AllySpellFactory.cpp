#include "AllySpellFactory.hpp"

AllySpellFactory::AllySpellFactory (size_t range) :
    range(range) {}

Spell* AllySpellFactory::createSpell (ChangeableFields& changeableFields) {
    RangeFields& rf = dynamic_cast<RangeFields&>(changeableFields);
    AllySpell* spell = new AllySpell(rf.getField(), {40, 20}, range, 1, rf.getCaster(), rf.getTarget());
    spell->upgradeSpell(rf.getHand().getHandLevel());

    return spell;
}