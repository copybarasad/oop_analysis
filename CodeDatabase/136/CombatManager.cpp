#include "CombatManager.h"
#include <iostream>
#include <algorithm>

CombatManager::CombatManager(Wanderer& wanderer, BeastManager& beastManager,
                            BuildingManager& buildingManager, 
                            GameField& field, const Position& wandererPos)
    : wanderer(wanderer), beastManager(beastManager), buildingManager(buildingManager),
      field(field), wandererPosition(wandererPos), killCounter(0),
      buildingCombat(wanderer, buildingManager, field, wandererPosition) {}

void CombatManager::processMeleeAttack(const Position& targetPos) {
    Beast* beast = beastManager.getBeastAt(targetPos);
    if (beast) {
        int damage = wanderer.getCurrentDamage();
        beast->takeDamage(damage);
        std::cout << "Melee attack! You hit beast for " << damage << " damage! ";
        
        if (!beast->isAlive()) {
            std::cout << "Beast defeated!\n";
            field.getCell(targetPos.x, targetPos.y).setHasBeast(false);
        } else {
            std::cout << "Beast health: " << beast->getHealth() << "\n";
        }
        
        if (wanderer.getIsAlive()) {
            wanderer.takeDamage(beast->getDamage());
            std::cout << "Beast counterattacks for " << beast->getDamage() << " damage!\n";
        }
    } else {
        buildingCombat.processMeleeBuildingAttack(targetPos);
    }
}

void CombatManager::processRangedAttack(int directionX, int directionY) {
    bool attacked = false;
    
    const auto& beasts = beastManager.getBeasts();
    
    for (const auto& beastData : beasts) {
        int beastX = beastData.first.x;
        int beastY = beastData.first.y;
        
        int vecX = beastX - wandererPosition.x;
        int vecY = beastY - wandererPosition.y;
        
        int distance = std::max(std::abs(vecX), std::abs(vecY));
        
        if (distance >= 2 && distance <= 3) {
            if ((directionX == 0 || (vecX * directionX > 0)) &&
                (directionY == 0 || (vecY * directionY > 0))) {

                if (!field.hasLineOfSight(wandererPosition.x, wandererPosition.y, beastX, beastY)) {
                    continue;
                }
                
                int damage = wanderer.getCurrentDamage();
                Beast* beast = beastManager.getBeastAt(Position(beastX, beastY));
                if (beast) {
                    beast->takeDamage(damage);
                    std::cout << "Ranged attack! You shoot beast for " << damage << " damage! ";
                    
                    if (!beast->isAlive()) {
                        std::cout << "Beast defeated!\n";
                        field.getCell(beastX, beastY).setHasBeast(false);
                    } else {
                        std::cout << "Beast health: " << beast->getHealth() << "\n";
                    }
                    attacked = true;
                    break;
                }
            }
        }
    }
    
    if (!attacked) {
        buildingCombat.processRangedBuildingAttack(directionX, directionY);
    }
}

void CombatManager::checkCombat() {
    int killedBeasts = beastManager.countDeadBeasts();
    
    if (killedBeasts > 0) {
        int points = killedBeasts * 50;
        wanderer.addPoints(points);
        int exp = killedBeasts * 50;
        wanderer.addExp(exp);
        std::cout << "Defeated " << killedBeasts << " beasts! +" << points << " points!\n";
        std::cout << "Experience gained: " << exp << "\n";
        
        checkSpellRewards(killedBeasts);
        beastManager.removeDeadBeasts();
    }
}

void CombatManager::checkSpellRewards(int killedBeasts) {
    killCounter += killedBeasts;
    if (killCounter >= 3) {
        killCounter = 0;
        
        SpellFactory factory;
        auto newSpell = factory.createRandomSpell();
        if (newSpell) {
            if (wanderer.getHand().getSpellCount() < wanderer.getHand().getMaxSize()) {
                if (wanderer.learnSpell(std::move(newSpell))) {
                    std::cout << "Reward: New spell earned for defeating 3 beasts!\n";
                }
            } else {
                std::cout << "Hand is full! Can't add new spell.\n";
            }
        }
    }
}

void CombatManager::updateWandererPosition(const Position& newPos) {
    wandererPosition = newPos;
}