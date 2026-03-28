#include "lib/TrapSpell.h"
#include "lib/GameField.h"
#include "lib/Player.h"
#include <cmath>
#include <iostream>

TrapSpell::TrapSpell(const std::string& spellName, 
                   int spellDamage, 
                   int spellManaCost, 
                   int spellRange,
                   int spellDuration) 
    : name(spellName), 
      manaCost(spellManaCost),
      range(spellRange),
      damage(spellDamage),
      duration(spellDuration) {
    description = "Places a trap that deals " + std::to_string(damage) + 
                 " damage when triggered. Lasts " + std::to_string(duration) + " turns";
}

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

bool TrapSpell::canCast(int casterX, int casterY, int targetX, int targetY, GameField* field) const {
    if (!field || !field->isValidPosition(targetX, targetY)) {
        return false;
    }
    
    double distance = std::sqrt(std::pow(targetX - casterX, 2) + std::pow(targetY - casterY, 2));
    if (distance > range) {
        return false;
    }
    
    // Can only place trap on empty, passable cells
    return field->isCellEmpty(targetX, targetY) && field->isCellPassable(targetX, targetY);
}

bool TrapSpell::cast(Player& caster, int targetX, int targetY, GameField* field) {
    if (!canCast(caster.getX(), caster.getY(), targetX, targetY, field)) {
        std::cout << "Cannot place " << name << " on target position!\n";
        return false;
    }
    
    std::cout << caster.getName() << " places " << name << " at (" << targetX << ", " << targetY << ")!\n";
    std::cout << "The trap will trigger when an enemy steps on it, dealing " << damage << " damage.\n";
    
    return true;
}

SpellCard* TrapSpell::clone() const {
    return new TrapSpell(*this);
}

int TrapSpell::getDamage() const {
    return damage;
}

int TrapSpell::getDuration() const {
    return duration;
}