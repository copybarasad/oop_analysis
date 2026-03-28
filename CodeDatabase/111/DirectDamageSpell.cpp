#include "DirectDamageSpell.h"
#include "GameBoard.h"
#include "GameActor.h"
#include "Foe.h"
#include "EnemyTower.h"
#include "Hero.h" 
#include <iostream>
#include <cmath>
#include <algorithm>

DirectDamageSpell::DirectDamageSpell(const std::string& spellName, int spellDamage, int cost, int spellRange)
    : name(spellName), damage(spellDamage), manaCost(cost), range(spellRange) {
    description = "Deals " + std::to_string(damage) + " damage to a single target within range " + std::to_string(range);
}

bool DirectDamageSpell::canCast(const Location& casterPos, const Location& targetPos, const GameBoard& board) const {
    if (!board.isLocationValid(targetPos)) {
        return false;
    }
    
    if (board.isObstacle(targetPos)) {
        return false;
    }
    
    GameActor* target = board.getActorAt(targetPos);
    if (!target) {
        return false;
    }
    
    Foe* enemy = dynamic_cast<Foe*>(target);
    EnemyTower* tower = dynamic_cast<EnemyTower*>(target);
    Hero* hero = dynamic_cast<Hero*>(target); 
    
    if ((!enemy || !enemy->isAlive()) && 
        (!tower || !tower->isAlive()) && 
        (!hero || !hero->isAlive())) { 
        return false;
    }
    

    int distance = std::abs(static_cast<int>(casterPos.row) - static_cast<int>(targetPos.row)) +
                   std::abs(static_cast<int>(casterPos.col) - static_cast<int>(targetPos.col));
    
    return distance <= range;
}

void DirectDamageSpell::cast(const Location& casterPos, const Location& targetPos, GameBoard& board) {
    std::cout << "DEBUG: DirectDamageSpell::cast called\n";
    
    GameActor* target = board.getActorAt(targetPos);
    if (target) {
        Foe* enemy = dynamic_cast<Foe*>(target);
        EnemyTower* tower = dynamic_cast<EnemyTower*>(target);
        
        if ((enemy && enemy->isAlive()) || (tower && tower->isAlive())) { 
            target->receiveDamage(damage);
            std::cout << name << " hit! " << damage << " damage dealt to target at ("
                      << targetPos.row << "," << targetPos.col << ")\n";
            
            if (enemy) {
                std::cout << "Enemy HP: " << enemy->getHitpoints() << "/" << enemy->getMaxHitpoints() << "\n";
                if (!enemy->isAlive()) {
                    std::cout << "Enemy defeated!\n";
                }
            } else if (tower) {
                std::cout << "Tower HP: " << tower->getHitpoints() << "/" << tower->getMaxHitpoints() << "\n";
                if (!tower->isAlive()) {
                    std::cout << "Tower destroyed!\n";
                }
            }
        } else {
            std::cout << name << " missed - target is not valid or already dead!\n";
        }
    } else {
        std::cout << name << " missed - no target at location!\n";
    }
    
    std::cout << "DEBUG: DirectDamageSpell::cast completed\n";
}