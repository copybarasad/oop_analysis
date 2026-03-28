#include "DamageSpell.h"
#include "../Game.h"
#include <iostream>

DamageSpell::DamageSpell(int spellDamage, int spellRange, const std::string& spellName, int cost)
                    : damage(spellDamage), range(spellRange), name(spellName), manaCost(cost) {
    description = "Deals " + std::to_string(spellDamage) + " damage to a single target";
}

bool DamageSpell::canTarget(const Game& game, const Position& casterPos,
                            const Position& target) const {
    if (casterPos.distanceTo(target) > range) {
        return false;
    }
    if (!game.getField().isPositionValid(target)) {
        return false;
    }
    return game.canTargetForDamage(target);
}

bool DamageSpell::cast(Game& game, const Position& target) const {
    return game.damageEntityAt(target, damage);
}

// === Getters ===
std::string DamageSpell::getName() const {
    return name;
}

std::string DamageSpell::getDescription() const {
    return description;
}

int DamageSpell::getManaCost() const {
    return manaCost;
}

int DamageSpell::getRange() const {
    return range;
}

int DamageSpell::getDamage() const {
    return damage;
}

char DamageSpell::getSymbol() const {
    return 'F';
}

// === Setters ===
void DamageSpell::setDamage(int d) {
    if (d < 0)
        return;
    damage = d;
}

void DamageSpell::upgrade() {
    int increase = static_cast<int>(damage * 0.50); // + 50%
    if (increase < 1)
        increase = 1;
    damage += increase;
}