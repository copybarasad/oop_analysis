#include "direct_damage_spell.h"
#include "game.h"
#include <cmath>
#include <iostream>

DirectDamageSpell::DirectDamageSpell(int spellDamage, int spellRange)
    : damage(spellDamage), range(spellRange) {}

bool DirectDamageSpell::cast(Game& game, int targetX, int targetY) {
    std::cout << "Casting " << getName() << " at (" << targetX << ", " << targetY << ")!" << std::endl;
    if (isValidTarget(game, targetX, targetY)) {
        bool hitEnemy = game.damageEnemyAtPosition(targetX, targetY, damage);
        bool hitBuilding = game.damageBuildingAtPosition(targetX, targetY, damage);
        
        if (hitEnemy || hitBuilding) {
            std::cout << "Hit target for " << damage << " damage!" << std::endl;
            return true;
        }
    }
    return false;
}

bool DirectDamageSpell::isValidTarget(const Game& game, int targetX, int targetY) const {
    int playerX = game.getPlayerPositionX();
    int playerY = game.getPlayerPositionY();
    
    int distanceX = std::abs(targetX - playerX);
    int distanceY = std::abs(targetY - playerY);

    if (!game.isPositionValid(targetX, targetY)) {
        std::cout << "Invalid position!" << std::endl;
        return false;
    }

    if (distanceX > range || distanceY > range) {
        std::cout << "Target is too far! Maximum range is " << range << " tiles." << std::endl;
        return false;
    }
    
    if (!game.hasEnemyOrBuildingAt(targetX, targetY)) {
        std::cout << "No enemy or building at target position!" << std::endl;
        return false;
    }
    return true;
}

std::string DirectDamageSpell::getName() const {
    return "Fire discharge";
}

std::string DirectDamageSpell::getDescription() const {
    return "Deals " + std::to_string(damage) + " damage to a single target within " + std::to_string(range) + " cells";
}

int DirectDamageSpell::getManaCost() const {
    return 5;
}

int DirectDamageSpell::getRange() const {
    return range;
}

void DirectDamageSpell::increaseDamage(int amount) {
    damage += amount;
}

int DirectDamageSpell::getDamage() const {
    return damage;
}