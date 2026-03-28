#include "trap_spell.h"
#include "game.h"
#include <cmath>
#include <iostream>

TrapSpell::TrapSpell(int spellDamage, int spellRange)
    : damage(spellDamage), range(spellRange) {}

bool TrapSpell::cast(Game& game, int targetX, int targetY) {
    if (isValidTarget(game, targetX, targetY)) {
        game.placeTrap(targetX, targetY, damage);
        std::cout << "Placing " << getName() << " at (" << targetX << ", " << targetY << ")!" << std::endl;
        return true;
    }
    else {
        return false;
    }
}

bool TrapSpell::isValidTarget(const Game& game, int targetX, int targetY) const {
    int playerX = game.getPlayerPositionX();
    int playerY = game.getPlayerPositionY();
    
    int distanceX = std::abs(targetX - playerX);
    int distanceY = std::abs(targetY - playerY);
    
    if (distanceX > range || distanceY > range) {
        std::cout << "Invalid position for " << getName() << "!" << std::endl;
        std::cout << "Position must be within " << range << " tiles, passable and unoccupied." << std::endl;
        return false;
    }
    

    bool isValidFlag = game.isPositionValid(targetX, targetY) && game.isPositionPassable(targetX, targetY) && !game.hasEnemyOrBuildingAt(targetX, targetY);
    
    if (!isValidFlag) {
        std::cout << "Invalid position for " << getName() << "!" << std::endl;
        std::cout << "Position must be within " << range << " tiles, passable and unoccupied." << std::endl;
        return false;
    }
    return true;
}

std::string TrapSpell::getName() const {
    return "Bear Trap";
}

std::string TrapSpell::getDescription() const {
    return "Places a trap that deals " + std::to_string(damage) + " damage when triggered";
}

int TrapSpell::getManaCost() const {
    return 3;
}

int TrapSpell::getRange() const {
    return range;
}

void TrapSpell::increaseDamage(int amount) {
    damage += amount;
}

int TrapSpell::getDamage() const {
    return damage;
}