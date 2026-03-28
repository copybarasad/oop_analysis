#include "lib/DirectDamageSpell.h"
#include "lib/GameField.h"
#include "lib/Player.h"
#include "lib/Enemy.h"
#include <cmath>
#include <iostream>

DirectDamageSpell::DirectDamageSpell(const std::string& spellName, int spellDamage, int spellManaCost, int spellRange) 
    : name(spellName), manaCost(spellManaCost),range(spellRange),damage(spellDamage) {
    description = "Deals " + std::to_string(damage) + " damage to a single target within range " + std::to_string(range);
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

bool DirectDamageSpell::canCast(int casterX, int casterY, int targetX, int targetY, GameField* field) const {
    if (!field || !field->isValidPosition(targetX, targetY)) {
        return false;
    }
    
    double distance = std::sqrt(std::pow(targetX - casterX, 2) + std::pow(targetY - casterY, 2));
    if (distance > range) {
        return false;
    }
    
    // Check if target cell has an enemy
    return field->getCell(targetX, targetY).getHasEnemy();
}

bool DirectDamageSpell::cast(Player& caster, int targetX, int targetY, GameField* field) {
    if (!canCast(caster.getX(), caster.getY(), targetX, targetY, field)) {
        std::cout << "Cannot cast " << name << " on target position!\n";
        return false;
    }
    
    std::cout << caster.getName() << " casts " << name << "!\n";
    std::cout << "Target at (" << targetX << ", " << targetY << ") receives " << damage << " damage!\n";
    
    // Note: In a complete implementation, we would find and damage the actual enemy
    return true;
}

SpellCard* DirectDamageSpell::clone() const {
    return new DirectDamageSpell(*this);
}