#include "direct_damage_spell.h"

DirectDamageSpell::DirectDamageSpell(int spellDamage, int spellRange) 
    : damage(spellDamage), range(spellRange) {
    name = "Fireball";
    description = "Deals " + std::to_string(damage) + " damage to a single target";
    manaCost = 2;
}

std::string DirectDamageSpell::getName() const {
    return name;
}

std::string DirectDamageSpell::getDescription() const {
    return description;
}

int DirectDamageSpell::getManaCost() const {
    return manaCost;
}

int DirectDamageSpell::getRange() const {
    return range;
}

bool DirectDamageSpell::requiresTarget() const {
    return true;
}

void DirectDamageSpell::cast() {
    // CombatManager
}

bool DirectDamageSpell::canCast(const Position& casterPosition, const Position& targetPosition) const {
    int distanceX = std::abs(targetPosition.getX() - casterPosition.getX());
    int distanceY = std::abs(targetPosition.getY() - casterPosition.getY());
    return distanceX <= range && distanceY <= range;
}

int DirectDamageSpell::getDamage() const {
    return damage;
}