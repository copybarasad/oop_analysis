#include "damage_spell.hpp"

DirectDamageSpell::DirectDamageSpell(int range_, int dmg) 
    : range(range_), damage(dmg) {}

std::string DirectDamageSpell::getName() const {
    return name;
}

int DirectDamageSpell::getRange() const {
    return range;
}

int DirectDamageSpell::getDamage() const {
    return damage;
}
