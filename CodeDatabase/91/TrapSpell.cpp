#include "TrapSpell.h"
#include "../Game.h"
#include <iostream>

TrapSpell::TrapSpell(int spellDamage, int spellRange, const std::string& spellName, int cost)
                : damage(spellDamage), range(spellRange), name(spellName), manaCost(cost) {
    description = "Places a trap that deals " + std::to_string(spellDamage) + " damage when enemy steps on it";
}

bool TrapSpell::canTarget(const Game& game, const Position& casterPos,
                          const Position& target) const {
    if (casterPos.distanceTo(target) > range) {
        return false;
    }
    if (!game.getField().isPositionValid(target)) {
        return false;
    }
    return game.canTargetForTrap(target);
}

bool TrapSpell::cast(Game& game, const Position& target) const {
    game.placeTrapAt(target, damage);
    return true;
}

// === Getters ===
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

int TrapSpell::getDamage() const {
    return damage;
}

char TrapSpell::getSymbol() const { 
    return '*';  // * - Trap
}

void TrapSpell::upgrade() {
    int increase = static_cast<int>(damage * 0.50); // + 50%
    if (increase < 1) increase = 1;
    damage += increase;
}