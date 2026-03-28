#include "SpellHand.h"
#include "GameManager.h"
#include <iostream>

SpellHand::SpellHand(size_t maxSpells) : maxSize(maxSpells) {}

bool SpellHand::addSpell(std::unique_ptr<Spell> spell) {
    if (spells.size() < maxSize && spell) {
        spells.push_back(std::move(spell));
        return true;
    }
    return false;
}

std::unique_ptr<Spell> SpellHand::removeSpell(size_t index) {
    if (index < spells.size()) {
        auto spell = std::move(spells[index]);
        spells.erase(spells.begin() + index);
        return spell;
    }
    return nullptr;
}

bool SpellHand::castSpell(size_t index, GameManager& gameManager, int targetX, int targetY, SpellEnhancementManager& enhancementManager) {
    if (index >= spells.size()) {
        std::cout << "No spell in slot " << (index + 1) << "!\n";
        return false;
    }
    
    int manaCost = spells[index]->getManaCost();
    std::string spellName = spells[index]->getName();

    if (enhancementManager.hasEnhancement()) {
        auto enhancedSpell = enhancementManager.applyEnhancement(std::move(spells[index]));
        if (enhancedSpell) {
            spells[index] = std::move(enhancedSpell);
        }
    }
    
    bool success = spells[index]->cast(gameManager, targetX, targetY);

    if (success) {
        spells.erase(spells.begin() + index);
    }
    
    return success;
}

Spell* SpellHand::getSpell(size_t index) const {
    return (index < spells.size()) ? spells[index].get() : nullptr;
}

size_t SpellHand::getSpellCount() const {
    return spells.size();
}

size_t SpellHand::getMaxSize() const {
    return maxSize;
}

bool SpellHand::isFull() const {
    return spells.size() >= maxSize;
}

void SpellHand::clear() {
    spells.clear();
}
