#include "DirectDamageSpell.h"

DirectDamageSpell::DirectDamageSpell(const std::string& spellName, int spellDamage, int spellRange)
    : name(spellName), damage(spellDamage), range(spellRange) {}

std::string DirectDamageSpell::getName() const {
    return name;
}

std::string DirectDamageSpell::getDescription() const {
    return name + " (Single): " + std::to_string(damage) + " dmg, Range: " + std::to_string(range);
}

int DirectDamageSpell::getDamage() const {
    return damage;
}

int DirectDamageSpell::getRange() const {
    return range;
}
