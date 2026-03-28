#include "AreaDamageSpell.h"
#include "GameBoard.h"
#include "GameActor.h"
#include "Foe.h"
#include "EnemyTower.h"
#include <iostream>
#include <cmath>
#include <algorithm>

AreaDamageSpell::AreaDamageSpell(const std::string& spellName, int spellDamage, int cost, int spellRange)
    : name(spellName), damage(spellDamage), manaCost(cost), range(spellRange) {
    description = "Deals " + std::to_string(damage) + " damage in a 2x2 area within range " + std::to_string(range);
}

bool AreaDamageSpell::canCast(const Location& casterPos, const Location& targetPos, const GameBoard& board) const {
    if (!board.isLocationValid(targetPos)) {
        std::cout << "Target location (" << targetPos.row << "," << targetPos.col << ") is outside the board!\n";
        return false;
    }
    
    if (board.isObstacle(targetPos)) {
        std::cout << "Target location (" << targetPos.row << "," << targetPos.col << ") is an obstacle!\n";
        return false;
    }
    
    int distance = std::abs(static_cast<int>(casterPos.row) - static_cast<int>(targetPos.row)) +
                   std::abs(static_cast<int>(casterPos.col) - static_cast<int>(targetPos.col));
    
    if (distance > range) {
        std::cout << "Target is too far! Distance: " << distance << ", Max range: " << range << "\n";
        return false;
    }
    
    for (size_t row = targetPos.row; row < targetPos.row + 2 && row < board.getWidth(); ++row) {
        for (size_t col = targetPos.col; col < targetPos.col + 2 && col < board.getHeight(); ++col) {
            Location areaLoc(row, col);
            if (board.isObstacle(areaLoc)) {
                std::cout << "Area contains obstacles, cannot cast area spell!\n";
                return false;
            }
        }
    }
    
    return true;
}

void AreaDamageSpell::cast(const Location& casterPos, const Location& targetPos, GameBoard& board) {
    std::cout << "Area damage spell cast at center (" << targetPos.row << "," << targetPos.col << ")\n";
    int targetsHit = 0;
    
    for (size_t row = targetPos.row; row < targetPos.row + 2 && row < board.getWidth(); ++row) {
        for (size_t col = targetPos.col; col < targetPos.col + 2 && col < board.getHeight(); ++col) {
            Location areaLoc(row, col);
            if (board.isLocationValid(areaLoc) && !board.isObstacle(areaLoc)) {
                GameActor* target = board.getActorAt(areaLoc);
                if (target) {
                    Foe* enemy = dynamic_cast<Foe*>(target);
                    EnemyTower* tower = dynamic_cast<EnemyTower*>(target);
                    
                    if ((enemy && enemy->isAlive()) || (tower && tower->isAlive())) {
                        target->receiveDamage(damage);
                        targetsHit++;
                        std::cout << "  Hit target at (" << areaLoc.row << "," << areaLoc.col 
                                  << ") for " << damage << " damage\n";
                    }
                }
            }
        }
    }
    
    if (targetsHit == 0) {
        std::cout << "Area spell hit no targets in the 2x2 area\n";
    } else {
        std::cout << "Area spell hit " << targetsHit << " target(s)\n";
    }
}