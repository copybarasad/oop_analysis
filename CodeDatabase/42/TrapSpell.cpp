#include "TrapSpell.h"

TrapSpell::TrapSpell(const std::string& spellName, int spellDamage, int spellRange)
    : name(spellName), damage(spellDamage), range(spellRange) {}

std::string TrapSpell::getName() const {
    return name;
}

std::string TrapSpell::getDescription() const {
    return name + " (Trap): " + std::to_string(damage) + " dmg, Range: " + std::to_string(range);
}

int TrapSpell::getDamage() const {
    return damage;
}

int TrapSpell::getRange() const {
    return range;
}
