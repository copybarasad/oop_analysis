#include "Spell.h"
#include <iostream>

SpellCard::SpellCard(std::unique_ptr<ISpell> spell) : spell(std::move(spell)), used(false) {}

bool SpellCard::cast(Position target, GameField& field) {
    if (used) {
        std::cout << "Spell already used this turn!\n";
        return false;
    }
    
    spell->cast(target, field);
    used = true;
    return true;
}

bool SpellCard::isUsed() const {
    return used;
}

void SpellCard::reset() {
    used = false;
}

std::string SpellCard::getName() const {
    return spell->getName();
}

std::string SpellCard::getDescription() const {
    return spell->getDescription();
}

int SpellCard::getManaCost() const {
    return spell->getManaCost();
}

int SpellCard::getRange() const {
    return spell->getRange();
}

SpellType SpellCard::getType() const {
    return spell->getType();
}