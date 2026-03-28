#include "TrapSpell.h"
#include <iostream>

TrapSpell::TrapSpell()
    : ConcreteSpell(SpellType::TRAP, "Trap", 5, 1, 1, 5) {}

void TrapSpell::cast(const Position& casterPos, const Position& targetPos) {
    int dx = abs(casterPos.getX() - targetPos.getX());
    int dy = abs(casterPos.getY() - targetPos.getY());
    int distance = dx + dy;
    
    if (distance <= range) {
        std::cout << "Placing Trap! Damage: " << damage << std::endl;
    } else {
        std::cout << "Target out of range!" << std::endl;
    }
}

Spell* TrapSpell::clone() const {
    return new TrapSpell(*this);
}