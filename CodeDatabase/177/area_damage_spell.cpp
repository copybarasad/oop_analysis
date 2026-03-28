#include "area_damage_spell.h"
#include <cmath>

AreaDamageSpell::AreaDamageSpell(int spellDamage, int spellRange) 
    : damage(spellDamage), range(spellRange) {
    name = "Firestorm";
    description = "Deals " + std::to_string(damage) + " damage in a 2x2 area";
    manaCost = 3;
}

std::string AreaDamageSpell::getName() const {
    return name;
}

std::string AreaDamageSpell::getDescription() const {
    return description;
}

int AreaDamageSpell::getManaCost() const {
    return manaCost;
}

int AreaDamageSpell::getRange() const {
    return range;
}

bool AreaDamageSpell::requiresTarget() const {
    return true;
}

void AreaDamageSpell::cast() {
    // CombatManager
}

bool AreaDamageSpell::canCast(const Position& casterPosition, const Position& targetPosition) const {
    int distanceX = std::abs(targetPosition.getX() - casterPosition.getX());
    int distanceY = std::abs(targetPosition.getY() - casterPosition.getY());
    return distanceX <= range && distanceY <= range;
}

int AreaDamageSpell::getDamage() const {
    return damage;
}

int AreaDamageSpell::getAreaSize() const {
    return AREA_SIZE;
}