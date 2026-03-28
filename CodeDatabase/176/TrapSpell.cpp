#include "TrapSpell.h"
#include "GameManager.h"
#include <iostream>
#include <cmath>

TrapSpell::TrapSpell(int spellDamage, int spellRange, int cost)
    : damage(spellDamage), range(spellRange), manaCost(cost) {
    name = "Bear Trap";
    description = "Places trap that deals 20 damage";
}

bool TrapSpell::cast(GameManager& gameManager, int targetX, int targetY) {
    auto playerPos = gameManager.getPlayerPosition();
    int distance = std::abs(targetX - playerPos.first) + std::abs(targetY - playerPos.second);
    
    if (distance > range) {
        std::cout << "Target out of range!\n";
        return false;
    }
    
    if (!gameManager.getField().isValidPosition(targetX, targetY)) {
        return false;
    }
    
    if (!gameManager.getField().isCellPassable(targetX, targetY)) {
        std::cout << "Cannot place trap there!\n";
        return false;
    }

    if (gameManager.getField().hasEnemyAt(targetX, targetY)) {
        gameManager.damageEnemyAt(targetX, targetY, damage);
        std::cout << name << " damages enemy at (" << targetX << ", " << targetY << ") for " << damage << " damage!\n";
        return true;
    }

    gameManager.placeTrapAt(targetX, targetY, damage);
    std::cout << name << " placed at (" << targetX << ", " << targetY << ")\n";
    return true;
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

bool TrapSpell::requiresTarget() const {
    return true;
}

int TrapSpell::getDamage() const {
    return damage;
}

int TrapSpell::getRange() const {
    return range;
}
