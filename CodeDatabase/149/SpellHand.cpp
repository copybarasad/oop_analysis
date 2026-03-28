#include "SpellHand.h"
#include "GameField copy.h"
#include <sstream>


SpellHand::SpellHand(int maxSpells) : maximumSize(maxSpells) {}

bool SpellHand::addSpell(std::unique_ptr<Spell> spell) {
    if (spells.size() >= maximumSize) {
        return false;
    }
    spells.push_back(std::move(spell));
    return true;
}

bool SpellHand::useSpell(int index, const Position& targetPosition, GameEngine& game) {
    if (index < 0 || index >= spells.size()) {
        return false;
    }
    return spells[index]->apply(targetPosition, game);
}

int SpellHand::getSpellCount() const {
    return spells.size();
}

int SpellHand::getMaxSize() const {
    return maximumSize;
}

std::string SpellHand::getSpellName(int index) const {
    if (index < 0 || index >= spells.size()) {
        return "";
    }
    return spells[index]->getName();
}

int SpellHand::getSpellManaCost(int index) const {
    if (index < 0 || index >= spells.size()) {
        return 0;
    }
    return spells[index]->getManaCost();
}

std::string SpellHand::getSpellInfo(int index) const {
    if (index < 0 || index >= spells.size()) {
        return "Invalid spell index";
    }
    
    std::stringstream info;
    info << "Name: " << spells[index]->getName()
         << ", Mana Cost: " << spells[index]->getManaCost()
         << ", " << spells[index]->getDescription();
    
    return info.str();
}