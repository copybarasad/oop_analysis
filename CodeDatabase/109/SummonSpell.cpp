#include "SummonSpell.h"
#include <iostream>

SummonSpell::SummonSpell()
    : ConcreteSpell(SpellType::SUMMON, "Summon", 0, 1, 1, 25), baseSummonCount(1) {}

int SummonSpell::getSummonCount() const {
    return baseSummonCount;
}

void SummonSpell::enhanceSummonCount(int amount) {
    baseSummonCount += amount;
}

void SummonSpell::cast(const Position& casterPos, const Position& targetPos) {
    std::cout << "Casting Summon! Will try to summon " << baseSummonCount << " ally nearby." << std::endl;
}

Spell* SummonSpell::clone() const {
    return new SummonSpell(*this);
}