#include "area_damage_spell.h"
#include "game.h"
#include <cmath>
#include <iostream>

AreaDamageSpell::AreaDamageSpell(int spellDamage, int spellRange)
    : damage(spellDamage), range(spellRange) {}

bool AreaDamageSpell::cast(Game& game, int targetX, int targetY) {
    bool anyHit = false;
    
    std::cout << "Casting " << getName() << " at area around (" << targetX << ", " << targetY << ")!" << std::endl;
    
    for (int x = targetX; x <= targetX + 1; ++x) {
        for (int y = targetY; y <= targetY + 1; ++y) {
            bool hitEnemy = game.damageEnemyAtPosition(x, y, damage);
            bool hitBuilding = game.damageBuildingAtPosition(x, y, damage);
            if (hitEnemy || hitBuilding) {
                anyHit = true;
            }
        }
    }
    
    if (anyHit) {
        std::cout << "Fireball hit targets for " << damage << " damage!" << std::endl;
    }
    else {
        std::cout << "Fireball exploded but hit nothing!" << std::endl;
    }
    
    return true;
}

bool AreaDamageSpell::isValidTarget(const Game& game, int targetX, int targetY) const {
    int playerX = game.getPlayerPositionX();
    int playerY = game.getPlayerPositionY();
    
    int distanceX = std::abs(targetX - playerX);
    int distanceY = std::abs(targetY - playerY);
    
    if (distanceX > range || distanceY > range) {
        std::cout << "Target is too far! Maximum range is " << range << " tiles." << std::endl;
        std::cout << "Invalid position!" << std::endl;
        
        return false;
    }

    for (int x = targetX; x <= targetX + 1; ++x) {
        for (int y = targetY; y <= targetY + 1; ++y) {
            if (game.isPositionValid(x, y)) {
                return true;
            }
        }
    }
    std::cout << "No valid cells in the target area!" << std::endl;
    return false;
}

std::string AreaDamageSpell::getName() const {
    return "Fireball";
}

std::string AreaDamageSpell::getDescription() const {
    return "Deals " + std::to_string(damage) + " damage in a 2x2 area (from target cell to bottom-right) within " + std::to_string(range) + " tiles";
}

int AreaDamageSpell::getManaCost() const {
    return 4;
}

int AreaDamageSpell::getRange() const {
    return range;
}

void AreaDamageSpell::increaseDamage(int amount) {
    damage += amount;
}

int AreaDamageSpell::getDamage() const {
    return damage;
}