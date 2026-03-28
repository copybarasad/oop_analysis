#include "FireballSpell.h"
#include <iostream>

FireballSpell::FireballSpell() 
    : ConcreteSpell(SpellType::FIREBALL, "Fireball", 15, 3, 1, 10) {}

void FireballSpell::cast(const Position& casterPos, const Position& targetPos) {
    int dx = abs(casterPos.getX() - targetPos.getX());
    int dy = abs(casterPos.getY() - targetPos.getY());
    int distance = dx + dy;
    
    if (distance <= range) {
        std::cout << "Casting Fireball! Damage: " << damage << std::endl;
    } else {
        std::cout << "Target out of range!" << std::endl;
    }
}

Spell* FireballSpell::clone() const {
    return new FireballSpell(*this);
}