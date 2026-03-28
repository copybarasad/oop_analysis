#include "SpellHand.h"
#include "Game.h"
#include <iostream>

SpellHand::SpellHand(int maxSize) : maxSize(maxSize) {}

bool SpellHand::addSpell(std::unique_ptr<Spell> spell) {
    if (isFull()) {
        std::cout << "Spell hand is full! Cannot add more spells." << std::endl;
        return false;
    }
    
    spells.push_back(std::move(spell));
    std::cout << "Added spell: " << spells.back()->getName() << std::endl;
    return true;
}

bool SpellHand::useSpell(int index, Game& game, int targetX, int targetY) {
    if (index < 0 || index >= spells.size()) {
        std::cout << "Invalid spell index!" << std::endl;
        return false;
    }
    
    Spell* spell = spells[index].get();
    bool success = game.castSpell(spell, targetX, targetY);
    
    if (success) {
        spells.erase(spells.begin() + index);
        return true;
    }
    
    return false;
}

void SpellHand::displaySpells() const {
    std::cout << "\n=== SPELLS ===" << std::endl;
    for (size_t i = 0; i < spells.size(); ++i) {
        std::cout << i + 1 << ". " << spells[i]->getName() 
                  << " - " << spells[i]->getDescription()
                  << " [Range: " << spells[i]->getRange() << "]" << std::endl;
    }
    if (spells.empty()) {
        std::cout << "No spells available." << std::endl;
    }
}

int SpellHand::getSpellCount() const { return spells.size(); }
bool SpellHand::isFull() const { return spells.size() >= maxSize; }

void SpellHand::removeSpell(int index) {
    if (index >= 0 && index < spells.size()) {
        spells.erase(spells.begin() + index);
    }
}

const std::vector<std::unique_ptr<Spell>>& SpellHand::getSpells() const {
    return spells;
}