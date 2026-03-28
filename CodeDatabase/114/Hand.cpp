#include <utility>
#include "Hand.h"
#include "spells/ISpell.h"

void Hand::setSpellCastDelayActive(bool spellCastDelayActive) {
    this->spellCastDelayActive = spellCastDelayActive;
}

void Hand::setPendingBoostMultiplier(int pendingBoostMultiplier) {
    this->pendingBoostMultiplier = pendingBoostMultiplier;
}

bool Hand::addSpell(SpellType spell) {
    if (spells.size() >= this->maxSpellsCount) {
        return false;
    }
    spells.push_back(std::move(spell));
    return true;
}

void Hand::removeSpell(std::size_t index) {
    if (index >= spells.size()) return;
    spells.erase(spells.begin() + index);
}
