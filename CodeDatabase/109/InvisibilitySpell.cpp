#include "InvisibilitySpell.h"
#include <iostream>

InvisibilitySpell::InvisibilitySpell()
    : ConcreteSpell(SpellType::INVISIBILITY, "Invisibility", 0, 0, 0, 20), baseDuration(3) {}

void InvisibilitySpell::cast(const Position& casterPos, const Position& targetPos) {
    std::cout << "Casting Invisibility! Duration: " << baseDuration << " turns" << std::endl;
}

Spell* InvisibilitySpell::clone() const {
    return new InvisibilitySpell(*this);
}

void InvisibilitySpell::enhanceDuration(int amount) {
    baseDuration += amount;
}

int InvisibilitySpell::getDuration() const {
    return baseDuration;
}