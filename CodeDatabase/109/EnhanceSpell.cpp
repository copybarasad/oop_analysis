#include "EnhanceSpell.h"
#include <iostream>

EnhanceSpell::EnhanceSpell()
    : ConcreteSpell(SpellType::ENHANCE, "Enhance", 0, 0, 0, 15), basePower(0) {}

void EnhanceSpell::cast(const Position& casterPos, const Position& targetPos) {
    std::cout << "Casting Enhance! Base power: " << basePower << std::endl;
}

Spell* EnhanceSpell::clone() const {
    return new EnhanceSpell(*this);
}

void EnhanceSpell::enhancePower(int amount) {
    basePower += amount;
}

int EnhanceSpell::getBasePower() const {
    return basePower;
}

void EnhanceSpell::setBasePower(int power) {
    basePower = power;
}