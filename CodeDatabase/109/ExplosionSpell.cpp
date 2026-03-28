#include "ExplosionSpell.h"
#include <iostream>

ExplosionSpell::ExplosionSpell()
    : ConcreteSpell(SpellType::EXPLOSION, "Explosion", 20, 1, 1, 15) {}

void ExplosionSpell::cast(const Position& casterPos, const Position& targetPos) {
    int dx = abs(casterPos.getX() - targetPos.getX());
    int dy = abs(casterPos.getY() - targetPos.getY());
    int distance = dx + dy;
    
    if (distance <= range) {
        std::cout << "Casting Explosion! Area: " << area << "x" << area << " Damage: " << damage << std::endl;
    } else {
        std::cout << "Target out of range!" << std::endl;
    }
}

Spell* ExplosionSpell::clone() const {
    return new ExplosionSpell(*this);
}