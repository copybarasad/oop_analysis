#include "aoe_spell.hpp"

AoeDamageSpell::AoeDamageSpell(int range_, int dmg)
    : range(range_), damage(dmg) {}

std::string AoeDamageSpell::getName() const {
    return name;
}

int AoeDamageSpell::getRange() const {
    return range;
}

int AoeDamageSpell::getDamage() const {
    return damage;
}
