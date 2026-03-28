#include "player_hand.h"
#include <cstdlib>
#include <cassert>

PlayerHand::PlayerHand(size_t maxSize) : maxSize(maxSize) {}

bool PlayerHand::addSpell(std::unique_ptr<Spell> spell) {
    if (spells.size() >= maxSize) {
        return false;
    }
    spells.push_back(std::move(spell));
    return true;
}

size_t PlayerHand::getSpellCount() const {
    return spells.size();
}

size_t PlayerHand::getMaxSize() const {
    return maxSize;
}

std::unique_ptr<Spell> PlayerHand::drawRandomSpell() {
    if (spells.empty()) {
        return nullptr;
    }
    
    size_t index = rand() % spells.size();
    std::unique_ptr<Spell> drawn = std::move(spells[index]);
    spells.erase(spells.begin() + index);
    
    return drawn;
}

const std::vector<std::unique_ptr<Spell>>& PlayerHand::getSpells() const {
    return spells;
}

bool PlayerHand::isFull() const {
    return spells.size() >= maxSize;
}

void PlayerHand::discardRandomHalf() {
    if (spells.empty()) return;
    size_t toRemove = spells.size() / 2;
    for (size_t i = 0; i < toRemove; ++i) {
        size_t idx = rand() % spells.size();
        spells.erase(spells.begin() + idx);
        if (spells.empty()) break;
    }
}