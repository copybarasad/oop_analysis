#include "TrapSpell.h"
#include "GameBoard.h"
#include "TrapActor.h"
#include <iostream>
#include <cmath>
#include <algorithm>

TrapSpell::TrapSpell(const std::string& spellName, int spellDamage, int cost, int spellRange)
    : name(spellName), damage(spellDamage), manaCost(cost), range(spellRange) {
    description = "Places a trap (*) that deals " + std::to_string(damage) + " damage when triggered within range " + std::to_string(range);
}

bool TrapSpell::canCast(const Location& casterPos, const Location& targetPos, const GameBoard& board) const {
    if (!board.isLocationValid(targetPos)) {
        std::cout << "Target location (" << targetPos.row << "," << targetPos.col << ") is outside the board!\n";
        return false;
    }
    
    if (board.isObstacle(targetPos)) {
        std::cout << "Target location (" << targetPos.row << "," << targetPos.col << ") is an obstacle!\n";
        return false;
    }
    
    if (board.getActorAt(targetPos)) {
        std::cout << "Target location (" << targetPos.row << "," << targetPos.col << ") is occupied by another actor!\n";
        return false;
    }
    
    int distance = std::abs(static_cast<int>(casterPos.row) - static_cast<int>(targetPos.row)) +
                   std::abs(static_cast<int>(casterPos.col) - static_cast<int>(targetPos.col));
    
    if (distance > range) {
        std::cout << "Target is too far! Distance: " << distance << ", Max range: " << range << "\n";
        return false;
    }
    
    return true;
}

void TrapSpell::cast(const Location& casterPos, const Location& targetPos, GameBoard& board) {
    std::cout << "DEBUG: TrapSpell::cast called\n";
    
    // Создаем ловушку
    TrapActor* trap = new TrapActor(targetPos, damage);
    board.addActor(trap);
    std::cout << "Trap placed at (" << targetPos.row << "," << targetPos.col << ") and will appear as * on map\n";
    
    std::cout << "DEBUG: TrapSpell::cast completed\n";
}