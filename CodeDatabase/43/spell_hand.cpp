#include "spell_hand.h"
#include <stdexcept>

SpellHand::SpellHand(size_t maxHandSize) : maxSize(maxHandSize) {}

size_t SpellHand::getSize() const {
    return spells.size();
}

size_t SpellHand::getMaxSize() const {
    return maxSize;
}

bool SpellHand::isFull() const {
    return spells.size() >= maxSize;
}

bool SpellHand::isEmpty() const {
    return spells.empty();
}

bool SpellHand::addSpell(std::shared_ptr<Spell> spell) {
    if (isFull()) {
        return false;
    }
    spells.push_back(spell);
    return true;
}

bool SpellHand::removeSpell(int index) {
    if (index < 0 || index >= static_cast<int>(spells.size())) {
        return false;
    }
    spells.erase(spells.begin() + index);
    return true;
}

std::shared_ptr<Spell> SpellHand::getSpell(int index) const {
    if (index < 0 || index >= static_cast<int>(spells.size())) {
        return nullptr;
    }
    return spells[index];
}

const std::vector<std::shared_ptr<Spell>>& SpellHand::getAllSpells() const {
    return spells;
}

void SpellHand::clear() {
    spells.clear();
}