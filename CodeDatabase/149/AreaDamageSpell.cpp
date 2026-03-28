#include "AreaDamageSpell.h"
#include "GameEngine.h"
#include <cmath>
#include <unordered_set>

AreaDamageSpell::AreaDamageSpell(int spellDamage, int spellRange, int cost) 
    : damageAmount(spellDamage), castRange(spellRange), manaCost(cost) {}

bool AreaDamageSpell::apply(const Position& targetPosition, GameEngine& game) {
    Position playerPosition = game.getPlayerPosition();
    
    int distance = std::abs(targetPosition.getX() - playerPosition.getX()) + 
                   std::abs(targetPosition.getY() - playerPosition.getY());
    
    if (distance > castRange) {
        return false;
    }
    
    bool hitAnyTarget = false;
    std::unordered_set<int> attackedEnemies;
    
    for (int dx = 0; dx < 2; dx++) {
        for (int dy = 0; dy < 2; dy++) {
            Position checkPosition(targetPosition.getX() + dx, targetPosition.getY() + dy);
            
            if (!game.isValidPosition(checkPosition)) {
                continue;
            }
            
            auto enemyPositions = game.getEnemyPositions();
            for (size_t i = 0; i < enemyPositions.size(); i++) {
                if (enemyPositions[i] == checkPosition) {
                    if (attackedEnemies.find(i) == attackedEnemies.end() && 
                        game.isEnemyAlive(i)) {
                        game.damageEnemy(i, damageAmount);
                        attackedEnemies.insert(i);
                        hitAnyTarget = true;
                    }
                    break;
                }
            }
            
            if (game.isEnemyBuildingAt(checkPosition)) {
                game.damageBuilding(checkPosition, damageAmount);
                hitAnyTarget = true;
            }
        }
    }
    
    return hitAnyTarget;
}

std::string AreaDamageSpell::getName() const {
    return "Area Damage";
}

int AreaDamageSpell::getManaCost() const {
    return manaCost;
}

std::string AreaDamageSpell::getDescription() const {
    return "Area Damage, Range: " + std::to_string(castRange) + ", Area: 2x2, Damage: " + std::to_string(damageAmount);
}