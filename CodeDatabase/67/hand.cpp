#include "hand.h"
#include <iostream>

Hand::Hand(int maximum_size) : max_size(maximum_size)
{}

Hand::~Hand() {
    for (Spell* spell : spells) {
        if (spell != nullptr) {
            delete spell;
        }
    }
    spells.clear();
}

bool Hand::addSpell(Spell* spell) {
    if (isFull()) {
        std::cout << "Hand is full! Cannot add more spells." << std::endl;
        return false;
    }
    
    spells.push_back(spell);
    std::cout << "Added spell: " << spell->getName() << std::endl;
    return true;
}

void Hand::clearSpells() {
    for (Spell* spell : spells) {
        if (spell != nullptr) {
            delete spell;
        }
    }
    spells.clear();
}

Spell* Hand::getSpell(int index) {
    if (index < 0 || index >= static_cast<int>(spells.size())) {
        return nullptr;
    }
    return spells[index];
}

bool Hand::removeSpell(int index) {
    if (index < 0 || index >= static_cast<int>(spells.size())) {
        return false;
    }
    
    delete spells[index];
    spells.erase(spells.begin() + index);
    return true;
}

int Hand::getSpellCount() const {
    return spells.size();
}

int Hand::getMaxSize() const {
    return max_size;
}

bool Hand::isFull() const {
    return spells.size() >= static_cast<size_t>(max_size);
}

void Hand::printSpells() const {
    std::cout << "=== Your Spells (Hand: " << spells.size() << "/" << max_size << ") ===" << std::endl;
    for (size_t i = 0; i < spells.size(); ++i) {
        std::cout << i + 1 << ". " << spells[i]->getName() << " (Damage: " << spells[i]->getDamage() << ", Radius: " << spells[i]->getRadius() << ")" << std::endl;
    }
    std::cout << "================================" << std::endl;
}
