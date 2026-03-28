#include "DirectDamageSpell.h"
#include "GameEngine.h"
#include <cmath>

DirectDamageSpell::DirectDamageSpell(int spellDamage, int spellRange, int cost) 
    : damageAmount(spellDamage), castRange(spellRange), manaCost(cost) {}

bool DirectDamageSpell::apply(const Position& targetPosition, GameEngine& game) {
    Position playerPosition = game.getPlayerPosition();
    
    int distance = std::abs(targetPosition.getX() - playerPosition.getX()) + 
                   std::abs(targetPosition.getY() - playerPosition.getY());
    
    if (distance > castRange) {
        return false;
    }
    
    auto enemyPositions = game.getEnemyPositions();
    for (size_t i = 0; i < enemyPositions.size(); i++) {
        if (enemyPositions[i] == targetPosition) {
            if (game.isEnemyAlive(i)) {
                game.damageEnemy(i, damageAmount);
                return true;
            }
        }
    }
    
    if (game.isEnemyBuildingAt(targetPosition)) {
        game.damageBuilding(targetPosition, damageAmount);
        return true;
    }
    
    return false;
}

std::string DirectDamageSpell::getName() const {
    return "Direct Damage";
}

int DirectDamageSpell::getManaCost() const {
    return manaCost;
}

std::string DirectDamageSpell::getDescription() const {
    return "Direct Damage, Range: " + std::to_string(castRange) + ", Damage: " + std::to_string(damageAmount);
}