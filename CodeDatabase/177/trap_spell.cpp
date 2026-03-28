#include "trap_spell.h"
#include <cmath>

TrapSpell::TrapSpell(int spellDamage, int spellRange) 
    : damage(spellDamage), range(spellRange) {
    name = "Bear Trap";
    description = "Places a trap that deals " + std::to_string(damage) + " damage when triggered";
    manaCost = 1;
}

std::string TrapSpell::getName() const {
    return name;
}

std::string TrapSpell::getDescription() const {
    return description;
}

int TrapSpell::getManaCost() const {
    return manaCost;
}

int TrapSpell::getRange() const {
    return range;
}

bool TrapSpell::requiresTarget() const {
    return true;
}

void TrapSpell::cast() {
    //CombatManager
}

bool TrapSpell::canCast(const Position& casterPosition, const Position& targetPosition) const {
    int distanceX = std::abs(targetPosition.getX() - casterPosition.getX());
    int distanceY = std::abs(targetPosition.getY() - casterPosition.getY());
    return distanceX <= range && distanceY <= range;
}

int TrapSpell::getDamage() const {
    return damage;
}