#include "lib/SpellHand.h"
#include <iostream>

SpellHand::SpellHand(int handSize) : maxSize(handSize), currentSize(0) {
    spells.resize(maxSize, nullptr);
}

SpellHand::~SpellHand() {
    clear();
}

SpellHand::SpellHand(SpellHand&& other) noexcept 
    : spells(std::move(other.spells)), maxSize(other.maxSize), currentSize(other.currentSize) {
    other.maxSize = 0;
    other.currentSize = 0;
}

SpellHand& SpellHand::operator=(SpellHand&& other) noexcept {
    if (this != &other) {
        clear();
        spells = std::move(other.spells);
        maxSize = other.maxSize;
        currentSize = other.currentSize;
        
        other.maxSize = 0;
        other.currentSize = 0;
    }
    return *this;
}

bool SpellHand::addSpell(SpellCard* spell) {
    if (currentSize >= maxSize || spell == nullptr) {
        return false;
    }
    
    for (int i = 0; i < maxSize; ++i) {
        if (spells[i] == nullptr) {
            spells[i] = spell->clone();
            currentSize++;
            return true;
        }
    }
    
    return false;
}

bool SpellHand::removeSpell(int index) {
    if (index < 0 || index >= maxSize || spells[index] == nullptr) {
        return false;
    }
    
    delete spells[index];
    spells[index] = nullptr;
    currentSize--;
    return true;
}

SpellCard* SpellHand::getSpell(int index) const {
    if (index < 0 || index >= maxSize) {
        return nullptr;
    }
    return spells[index];
}

int SpellHand::getSpellCount() const {
    return currentSize;
}

int SpellHand::getMaxSize() const {
    return maxSize;
}

void SpellHand::displayHand() const {
    std::cout << "\n=== Spell Hand ===\n";
    for (int i = 0; i < maxSize; ++i) {
        std::cout << i + 1 << ". ";
        if (spells[i] != nullptr) {
            std::cout << spells[i]->getName() << " (Mana: " << spells[i]->getManaCost() 
                      << ", Range: " << spells[i]->getRange() << ")";
        } else {
            std::cout << "Empty slot";
        }
        std::cout << "\n";
    }
    std::cout << "==================\n";
}

void SpellHand::clear() {
    for (auto& spell : spells) {
        delete spell;
        spell = nullptr;
    }
    currentSize = 0;
}