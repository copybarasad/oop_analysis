#include "TrapSpell.h"
#include "GameEngine.h"
#include <cmath>

TrapSpell::TrapSpell(int trapDamage, int trapRange, int cost) 
    : damageAmount(trapDamage), castRange(trapRange), manaCost(cost) {}

bool TrapSpell::apply(const Position& targetPosition, GameEngine& game) {
    Position playerPosition = game.getPlayerPosition();
    
    int distance = std::abs(targetPosition.getX() - playerPosition.getX()) + 
                   std::abs(targetPosition.getY() - playerPosition.getY());
    
    if (distance > castRange) {
        return false;
    }
    
    return game.placeTrap(targetPosition, damageAmount);
}

std::string TrapSpell::getName() const {
    return "Trap";
}

int TrapSpell::getManaCost() const {
    return manaCost;
}

std::string TrapSpell::getDescription() const {
    return "Trap, Range: " + std::to_string(castRange) + ", Damage: " + std::to_string(damageAmount);
}