#include "AreaDamageSpell.h"

AreaDamageSpell::AreaDamageSpell(const std::string& spellName, int spellDamage, int spellRange, int areaSize)
    : name(spellName), damage(spellDamage), range(spellRange), areaSize(areaSize) {}

std::string AreaDamageSpell::getName() const {
    return name;
}

std::string AreaDamageSpell::getDescription() const {
    return name + " (Area): " + std::to_string(damage) + " dmg, Area: " + 
           std::to_string(areaSize) + "x" + std::to_string(areaSize) + ", Range: " + std::to_string(range);
}

int AreaDamageSpell::getDamage() const {
    return damage;
}

int AreaDamageSpell::getRange() const {
    return range;
}

int AreaDamageSpell::getAreaSize() const {
    return areaSize;
}
