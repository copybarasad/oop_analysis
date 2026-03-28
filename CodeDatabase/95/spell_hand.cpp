#include "spell_hand.hpp"
#include <iostream>

SpellHand::SpellHand(std::vector<SpellCard*> initialSpells) : maxSize(6) {
    for (auto& spell : initialSpells) {
        addSpell(spell);
    }
}

bool SpellHand::addSpell(SpellCard* spell) {
    if (spells.size() >= maxSize) return false;
    spells.push_back(std::move(spell));
    return true;
}

SpellCard* SpellHand::getSpell(size_t index) {
    if (index >= spells.size()) return nullptr;
    return spells[index];
}

const SpellCard* SpellHand::getSpell(size_t index) const {
    if (index >= spells.size()) return nullptr;
    return spells[index];
}

void SpellHand::removeSpell(size_t index) {
    if (index < spells.size())
        spells.erase(spells.begin() + index);
}

void SpellHand::listSpells() const {
    if (spells.empty()) {
        std::cout << "Рука пуста.\n";
        return;
    }
    for (size_t i = 0; i < spells.size(); ++i) {
        std::cout << i << ": " << spells[i]->getName()
                    << " (Дальность: " << spells[i]->getRange() << ")\n";
    }
}

size_t SpellHand::getSize() const {
    return spells.size();
}

size_t SpellHand::getMaxSize() const {
    return maxSize;
}
