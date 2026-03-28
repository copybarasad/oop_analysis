#include "Hand.h"
#include <iostream>
#include <stdexcept>

Hand::Hand(int maxSize) : maxSize(maxSize) {
    if (maxSize <= 0) {
        throw std::invalid_argument("Hand size must be positive");
    }
}

Hand::Hand(const Hand &other) : maxSize(other.maxSize) {
    for (Spell *spell: other.spells) {
        if (spell) {
            spells.push_back(spell->clone());
        }
    }
}

Hand &Hand::operator=(const Hand &other) {
    if (this != &other) {
        for (Spell *spell: spells) {
            delete spell;
        }
        spells.clear();

        maxSize = other.maxSize;
        for (Spell *spell: other.spells) {
            if (spell) {
                spells.push_back(spell->clone());
            }
        }
    }
    return *this;
}

Hand::~Hand() {
    for (Spell *spell: spells) {
        delete spell;
    }
    spells.clear();
}

bool Hand::addSpell(Spell *spell) {
    if (!spell) {
        return false;
    }
    if (spells.size() < static_cast<size_t>(maxSize)) {
        spells.push_back(spell);
        return true;
    }
    return false;
}

void Hand::removeSpell(int index) {
    if (index < 0 || static_cast<size_t>(index) >= spells.size()) {
        throw std::out_of_range("Invalid spell index");
    }
    delete spells[index];
    spells.erase(spells.begin() + index);
}

Spell *Hand::getSpell(int index) const {
    if (index < 0 || static_cast<size_t>(index) >= spells.size()) {
        return nullptr;
    }
    return spells[index];
}

int Hand::getSize() const {
    return spells.size();
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

void Hand::display() const {
    std::cout << "Spells in hand (" << spells.size() << "/" << maxSize << "):\n";
    for (size_t i = 0; i < spells.size(); ++i) {
        if (spells[i]) {
            std::cout << i << ". " << spells[i]->getName()
                    << " - " << spells[i]->getDescription();
            if (spells[i]->requiresTarget() && spells[i]->getRange() > 0) {
                std::cout << " [Range: " << spells[i]->getRange() << "]";
            }
            std::cout << "\n";
        }
    }
    if (spells.empty()) {
        std::cout << "No spells available\n";
    }
}
