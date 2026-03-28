#include "SpellHand.hpp"

SpellHand::SpellHand (size_t handCapacity, const std::string spellName, size_t level) : level(level) {
    hand.reserve(handCapacity);
    hand.push_back(spellName);
}

bool SpellHand::addSpell (const std::string spellName) {
    if (isFull()) {
        return false;
    }

    hand.push_back(spellName);
    return true;
}

void SpellHand::removeSpell (const std::string spellName) {
    auto it = std::find(hand.begin(), hand.end(), spellName);
    if (it != hand.end()) {
        hand.erase(it);
    }
}

const std::string& SpellHand::getSpellName (size_t index) const {
    if (index >= hand.size()) {
        static std::string empty = "";
        return empty;
    }

    return hand[index];
}

void SpellHand::upgradeHand (size_t levels) { level += levels; }

void SpellHand::restoreHand () { level = 0; }

size_t SpellHand::getHandLevel () const { return level; }

size_t SpellHand::getSpellNumber () const { return hand.size(); }

bool SpellHand::isFull () const { return hand.size() == hand.capacity(); }

size_t SpellHand::getHandCapacity () const { return hand.capacity(); }