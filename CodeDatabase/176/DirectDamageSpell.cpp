#include "DirectDamageSpell.h"
#include "GameManager.h"
#include <iostream>
#include <cmath>

DirectDamageSpell::DirectDamageSpell(int spellDamage, int spellRange, int cost)
    : damage(spellDamage), range(spellRange), manaCost(cost) {
    name = "Fire Bolt";
    description = "Deals " + std::to_string(damage) + " damage to a single target";
}

bool DirectDamageSpell::cast(GameManager& gameManager, int targetX, int targetY) {
    auto playerPos = gameManager.getPlayerPosition();
    int distance = std::abs(targetX - playerPos.first) + std::abs(targetY - playerPos.second);
    
    if (distance > range) {
        std::cout << "Target out of range! (max: " << range << ")\n";
        return false;
    }
    
    if (!gameManager.getField().isValidPosition(targetX, targetY)) {
        std::cout << "Invalid target position!\n";
        return false;
    }

    if (gameManager.getField().hasEnemyAt(targetX, targetY)) {
        gameManager.damageEnemyAt(targetX, targetY, damage);
        std::cout << name << " hits enemy at (" << targetX << ", " << targetY << ") for " << damage << " damage!\n";
        return true;
    }
    
    std::cout << "No enemy at target position!\n";
    return false;
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

bool DirectDamageSpell::requiresTarget() const {
    return true;
}

int DirectDamageSpell::getDamage() const {
    return damage;
}

int DirectDamageSpell::getRange() const {
    return range;
}
