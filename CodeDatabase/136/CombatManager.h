#ifndef COMBATMANAGER_H
#define COMBATMANAGER_H

#include "Wanderer.h"
#include "BeastManager.h"
#include "BuildingManager.h"
#include "BuildingCombat.h"
#include "GameField.h"
#include "SpellFactory.h"
#include <vector>

class CombatManager {
private:
    Wanderer& wanderer;
    BeastManager& beastManager;
    BuildingManager& buildingManager;
    GameField& field;
    Position wandererPosition;
    int killCounter;
    BuildingCombat buildingCombat;

public:
    CombatManager(Wanderer& wanderer, BeastManager& beastManager, 
                 BuildingManager& buildingManager,
                 GameField& field, const Position& wandererPos);
    
    void processMeleeAttack(const Position& targetPos);
    void processRangedAttack(int directionX, int directionY);
    void checkCombat();
    void checkSpellRewards(int killedBeasts);
    void updateWandererPosition(const Position& newPos);
};

#endif