#include "Hand.h"
#include <random>

bool Hand::addSpell(std::unique_ptr<Spell> spell) {
    if (spells.size() < maxSize) {
        spells.push_back(std::move(spell));
        return true;
    }
    return false;
}

bool Hand::removeSpell(int index) {
    if (index >= 0 && index < spells.size()) {
        spells.erase(spells.begin() + index);
        return true;
    }
    return false;
}

Spell* Hand::getSpell(int index) const {
    if (index >= 0 && index < spells.size()) {
        return spells[index].get();
    }
    return nullptr;
}


Hand::Hand(const Hand& other) : maxSize(other.maxSize) {
}

Hand::Hand(Hand&& other) noexcept 
    : spells(std::move(other.spells)), maxSize(other.maxSize) {}

Hand& Hand::operator=(const Hand& other) {
    if (this != &other) {
        spells.clear();
        maxSize = other.maxSize;

    }
    return *this;
}

Hand& Hand::operator=(Hand&& other) noexcept {
    if (this != &other) {
        spells = std::move(other.spells);
        maxSize = other.maxSize;
    }
    return *this;
}