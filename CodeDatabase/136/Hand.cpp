#include "Hand.h"
#include <iostream>

Hand::Hand(int maxSize) : maxSize(maxSize), currentSize(0) {
    spells.reserve(maxSize);
}

bool Hand::addSpell(std::unique_ptr<SpellCard> spell) {
    if (!spell) {
        return false;
    }
    
    if (currentSize >= maxSize) {
        return false;
    }
    
    spells.push_back(std::move(spell));
    currentSize++;
    return true;
}

bool Hand::removeSpell(int index) {
    if (index < 0 || index >= currentSize) {
        return false;
    }
    
    spells.erase(spells.begin() + index);
    currentSize--;
    return true;
}

bool Hand::castSpell(int index, Position target, GameField& field) {
    if (index < 0 || index >= currentSize) {
        std::cout << "Invalid spell index!\n";
        return false;
    }
    
    return spells[index]->cast(target, field);
}

void Hand::resetSpells() {
    for (auto& spell : spells) {
        spell->reset();
    }
}

int Hand::getSpellCount() const {
    return currentSize;
}

int Hand::getMaxSize() const {
    return maxSize;
}

SpellCard* Hand::getSpell(int index) {
    if (index < 0 || index >= currentSize) return nullptr;
    return spells[index].get();
}

const SpellCard* Hand::getSpell(int index) const {
    if (index < 0 || index >= currentSize) return nullptr;
    return spells[index].get();
}

void Hand::displayHand() const {
    std::cout << "=== Spell Hand ===\n";
    for (int i = 0; i < currentSize; ++i) {
        std::cout << i + 1 << ". " << spells[i]->getName() 
                  << " (Cost: " << spells[i]->getManaCost() 
                  << ", Range: " << spells[i]->getRange() << ")\n";
        if (spells[i]->isUsed()) {
            std::cout << " [USED]";
        } else {
            std::cout << " [READY]";
        }
        
        std::cout << "   " << spells[i]->getDescription() << "\n";
    }
    if (currentSize == 0) {
        std::cout << "Empty\n";
    }
    std::cout << "==================\n";
}