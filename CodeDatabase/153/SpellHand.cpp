#include "SpellHand.h"
#include "Spell.h"
#include <iostream>

SpellHand::SpellHand(int size) : maxSize(size) {
}

bool SpellHand::addSpell(std::unique_ptr<Spell> spell) {
    if (spells.size() >= maxSize) {
        std::cout << "Hand is full! Cannot add new spell." << std::endl;
        return false;
    }
    spells.push_back(std::move(spell));
    std::cout << "Added spell: " << spells.back()->getName() << std::endl;
    return true;
}

bool SpellHand::removeSpell(int index) {
    if (index < 0 || index >= spells.size()) {
        return false;
    }
    spells.erase(spells.begin() + index);
    return true;
}

void SpellHand::clearHand() {
    spells.clear();
}

bool SpellHand::castSpell(int index, const SpellTarget& target,
    GameField& field, std::vector<Enemy>& enemies, Player& player) const {
    if (index < 0 || index >= spells.size()) {
        return false;
    }
    return spells[index]->cast(target, field, enemies, player);
}

std::string SpellHand::getSpellName(int index) const {
    if (index < 0 || index >= spells.size()) {
        return "";
    }
    return spells[index]->getName();
}

std::string SpellHand::getSpellDescription(int index) const {
    if (index < 0 || index >= spells.size()) {
        return "";
    }
    return spells[index]->getDescription();
}

int SpellHand::getSpellManaCost(int index) const {
    if (index < 0 || index >= spells.size()) {
        return 0;
    }
    return spells[index]->getManaCost();
}

int SpellHand::getSpellCount() const {
    return spells.size();
}

int SpellHand::getMaxSize() const {
    return maxSize;
}

bool SpellHand::isFull() const {
    return spells.size() >= maxSize;
}

void SpellHand::displaySpells() const {
    std::cout << "=== Spells in hand ===" << std::endl;
    for (int i = 0; i < spells.size(); i++) {
        std::cout << i + 1 << ". " << spells[i]->getDescription() << std::endl;
    }
    std::cout << "Free slots: " << (maxSize - spells.size()) << "/" << maxSize << std::endl;
}
