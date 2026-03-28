#include "Hand.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include <cstdlib>
#include <ctime>

Hand::Hand(int maxSize) : maxSize(maxSize) {
    if (maxSize < 1) {
        this->maxSize = 1;
    }
    
    spells.push_back(createRandomSpell());
}

Hand::Hand(const Hand& other) : maxSize(other.maxSize) {
    spells.reserve(other.spells.size());
    for (const auto& spell : other.spells) {
        if (spell) {
            spells.push_back(spell->clone());
        }
    }
}

Hand& Hand::operator=(const Hand& other) {
    if (this == &other) {
        return *this;
    }
    
    spells.clear();
    maxSize = other.maxSize;
    
    spells.reserve(other.spells.size());
    for (const auto& spell : other.spells) {
        if (spell) {
            spells.push_back(spell->clone());
        }
    }
    
    return *this;
}

std::unique_ptr<Spell> Hand::createRandomSpell() const {
    int spellType = std::rand() % 2;
    
    if (spellType == 0) {
        return std::make_unique<DirectDamageSpell>(3, 20);
    } else {
        return std::make_unique<AreaDamageSpell>(3, 15);
    }
}

bool Hand::addSpell(std::unique_ptr<Spell> spell) {
    if (!spell) return false;
    if (isFull()) return false;
    
    spells.push_back(std::move(spell));
    return true;
}

Spell* Hand::getSpell(int index) const {
    if (index < 0 || index >= static_cast<int>(spells.size())) {
        return nullptr;
    }
    return spells[index].get();
}

int Hand::getSpellCount() const {
    return static_cast<int>(spells.size());
}

int Hand::getMaxSize() const {
    return maxSize;
}

bool Hand::isFull() const {
    return spells.size() >= static_cast<size_t>(maxSize);
}

bool Hand::isEmpty() const {
    return spells.empty();
}

bool Hand::removeSpell(int index) {
    if (index < 0 || index >= static_cast<int>(spells.size())) {
        return false;
    }
    spells.erase(spells.begin() + index);
    return true;
}

