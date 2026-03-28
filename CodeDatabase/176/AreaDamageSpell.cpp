#include "AreaDamageSpell.h"
#include "GameManager.h"
#include <iostream>
#include <cmath>

AreaDamageSpell::AreaDamageSpell(int spellDamage, int spellRange, int spellAreaSize, int cost)
    : damage(spellDamage), range(spellRange), areaSize(spellAreaSize), manaCost(cost) {
    name = "Fireball";
    description = "Deals " + std::to_string(damage) + " damage in " + std::to_string(areaSize) + "x" + std::to_string(areaSize) + " area";
}

bool AreaDamageSpell::cast(GameManager& gameManager, int targetX, int targetY) {
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

    bool hit = gameManager.damageEnemiesInArea(targetX, targetY, areaSize, damage);
    std::cout << name << " explodes at (" << targetX << ", " << targetY << ")!\n";

    return true;
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

bool AreaDamageSpell::requiresTarget() const {
    return true;
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
