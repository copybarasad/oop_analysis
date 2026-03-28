#include "SpellHand.h"
#include <iostream>
#include <algorithm>
#include "SummonSpell.h"
#include "EnhanceSpell.h"   

SpellHand::SpellHand(int maxSize) : maxSize(maxSize), selectedSpell(0) {}

SpellHand::~SpellHand() {
    for (auto spell : spells) {
        delete spell;
    }
    spells.clear();
}

SpellHand::SpellHand(const SpellHand& other) : maxSize(other.maxSize), selectedSpell(other.selectedSpell) {
    for (auto spell : other.spells) {
        spells.push_back(spell->clone());
    }
}

SpellHand& SpellHand::operator=(const SpellHand& other) {
    if (this != &other) {
        for (auto spell : spells) {
            delete spell;
        }
        spells.clear();

        maxSize = other.maxSize;
        selectedSpell = other.selectedSpell;
        for (auto spell : other.spells) {
            spells.push_back(spell->clone());
        }
    }
    return *this;
}

bool SpellHand::addSpell(Spell* spell) {
    if (!spell) {
        return false; 
    }
    
    if (spells.size() >= maxSize) {
        return false;
    }

    for (auto existingSpell : spells) {
        if (existingSpell->getType() == spell->getType()) {
            return false; // Уже есть
        }
    }

    spells.push_back(spell);
    return true;
}

bool SpellHand::removeSpell(int index) {
    if (index >= 0 && index < spells.size()) {
        delete spells[index];
        spells.erase(spells.begin() + index);
        
        // Корректируем выбранное заклинание если нужно
        if (selectedSpell >= spells.size()) {
            selectedSpell = spells.size() - 1;
        }
        if (selectedSpell < 0 && !spells.empty()) {
            selectedSpell = 0;
        }
        return true;
    }
    return false;
}

Spell* SpellHand::getSpell(int index) const {
    if (index >= 0 && index < spells.size()) {
        return spells[index];
    }
    return nullptr;
}

int SpellHand::getSpellCount() const {
    return spells.size();
}

int SpellHand::getMaxSize() const {
    return maxSize;
}

void SpellHand::selectSpell(int index) {
    if (index >= 0 && index < spells.size()) {
        selectedSpell = index;
    }
}

Spell* SpellHand::getSelectedSpell() const {
    if (selectedSpell >= 0 && selectedSpell < spells.size()) {
        return spells[selectedSpell];
    }
    return nullptr;
}

Spell* SpellHand::createRandomSpell() {
    int randomValue = rand() % 10;
    
    if (randomValue < 0) {//4
        return new FireballSpell();
    } else if (randomValue < 3) { //6
        return new ExplosionSpell();
    } else if (randomValue < 6) { //7
        return new TrapSpell();
    } else if (randomValue < 0) { //8
        return new InvisibilitySpell();
    } else if (randomValue < 0) { //9
        return new SummonSpell();
    } else {
        return new EnhanceSpell();
    }
}