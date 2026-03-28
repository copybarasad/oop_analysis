#include "SpellCard.h"
#include <stdexcept>

SpellCard::SpellCard(std::unique_ptr<Spell> spellPtr)
    : spell(std::move(spellPtr)) {
    if (!spell) {
        throw std::invalid_argument("Spell pointer cannot be null");
    }
}

Spell* SpellCard::getSpell() const {
    return spell.get();
}
