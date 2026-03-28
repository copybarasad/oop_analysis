#include "PlayerHand.h"
#include <iostream>

PlayerHand::PlayerHand() {}

PlayerHand::PlayerHand(const std::vector<CommonSpell>& initialSpells) {
    for (const auto& spell : initialSpells) {
        addSpell(spell);
    }
}

void PlayerHand::addSpell(const CommonSpell& spell) {
    spells.push_back(spell);
}

bool PlayerHand::removeSpell(const std::string& spellName) {
    auto it = std::find_if(spells.begin(), spells.end(),
        [&spellName](const Spell& spell) {
            return spell.getName() == spellName;
        });

    if (it != spells.end()) {
        spells.erase(it);
        return true;
    }
    return false;
}

bool PlayerHand::removeSpell(int index) {
    if (index >= 0 && index < static_cast<int>(spells.size())) {
        spells.erase(spells.begin() + index);
        return true;
    }
    return false;
}

void PlayerHand::clearHand() {
    spells.clear();
}

CommonSpell& PlayerHand::getSpell(int index) {
    if (index < 0 || index >= static_cast<int>(spells.size())) {
        std::cout << index ;
        throw std::out_of_range("Invalid spell index");
    }
    return spells[index];
}

const CommonSpell& PlayerHand::getSpell(int index) const {
    if (index < 0 || index >= static_cast<int>(spells.size())) {
        throw std::out_of_range("Invalid spell index");
    }
    return spells[index];
}

std::vector<CommonSpell> PlayerHand::getAllSpells() const {
    return spells;
}
