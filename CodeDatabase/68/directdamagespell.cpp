#include "directdamagespell.h"
#include "gamefield.h"
#include <iostream>
#include <cmath>

DirectDamageSpell::DirectDamageSpell(int damage, int manaCost, int range) 
    : damage(damage), manaCost(manaCost), range(range) {}

void DirectDamageSpell::cast(GameField* field, const Position& targetPosition) {
    std::cout << "Casting " << getName() << " at position (" 
              << targetPosition.x << ", " << targetPosition.y << ")\n";
    
    // If field is provided, check range and valid targets
    if (field) {
        Position playerPos = field->getPlayer()->getPosition();
        
        // FIX: Use Manhattan distance for grid movement
        int manhattanDistance = abs(playerPos.x - targetPosition.x) + 
                               abs(playerPos.y - targetPosition.y);
        
        std::cout << "Player at (" << playerPos.x << "," << playerPos.y 
                  << "), Distance: " << manhattanDistance 
                  << ", Range: " << range << "\n";
        
        if (manhattanDistance > range) {
            std::cout << "Target out of range! Range: " << range 
                      << ", Distance: " << manhattanDistance << " - Spell not used!\n";
            return;
        }
        
        if (targetPosition == playerPos) {
            field->getPlayer()->takeDamage(damage);
            std::cout << "Hit player! " << damage << " damage dealt!\n";
        } 
        else if (field->hasEnemyAt(targetPosition) || field->hasBuildingAt(targetPosition)) {
            field->applyDamageAt(targetPosition, damage);
            std::cout << "Hit target! " << damage << " damage dealt!\n";
        } else {
            std::cout << "No valid target - spell wasted!\n";
        }
    } else {
        // Tower casting - just deal damage directly
        std::cout << "Tower's weakened spell hits for " << damage << " damage!\n";
    }
}

std::string DirectDamageSpell::getName() const {
    return "Direct Damage Spell";
}

int DirectDamageSpell::getManaCost() const {
    return manaCost;
}

int DirectDamageSpell::getDamage() const {
    return damage;
}

int DirectDamageSpell::getRange() const {
    return range;
}

void DirectDamageSpell::upgradeDamage(int additionalDamage) {
    damage += additionalDamage;
    std::cout << "Direct Damage Spell upgraded! Damage: " << damage << "\n";
}

void DirectDamageSpell::upgradeRange(int additionalRange) {
    range += additionalRange;
    std::cout << "Direct Damage Spell range increased! Range: " << range << "\n";
}

void DirectDamageSpell::reduceManaCost(int reduction) {
    manaCost = std::max(1, manaCost - reduction);
    std::cout << "Direct Damage Spell mana cost reduced! Cost: " << manaCost << "\n";
}