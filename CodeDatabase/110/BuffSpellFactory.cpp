#include "BuffSpellFactory.hpp"

BuffSpellFactory::BuffSpellFactory (size_t level) :
    level(level) {}

Spell* BuffSpellFactory::createSpell (ChangeableFields& changeableFields) {
    BuffFields& bf = dynamic_cast<BuffFields&>(changeableFields);
    BuffSpell* spell = new BuffSpell(bf.getHand(), level);
    spell->upgradeSpell(bf.getHand().getHandLevel());

    return spell;
}