#include "trap_spell.hpp"

TrapSpell::TrapSpell(int range_, int dmg)
    : range(range_), damage(dmg) {}

std::string TrapSpell::getName() const {
    return name;
}

int TrapSpell::getRange() const {
    return range;
}

int TrapSpell::getDamage() const {
    return damage;
}
