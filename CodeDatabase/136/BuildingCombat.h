#ifndef BUILDINGCOMBAT_H
#define BUILDINGCOMBAT_H

#include "Wanderer.h"
#include "BuildingManager.h"
#include "GameField.h"
#include <iostream>

class BuildingCombat {
private:
    Wanderer& wanderer;
    BuildingManager& buildingManager;
    GameField& field;
    Position wandererPosition;

public:
    BuildingCombat(Wanderer& wanderer, BuildingManager& buildingManager, 
                   GameField& field, const Position& wandererPos);
    
    void processMeleeBuildingAttack(const Position& targetPos);
    void processRangedBuildingAttack(int directionX, int directionY);
    void updateWandererPosition(const Position& newPos);

private:
    void destroyBuildingReward(const Position& pos, bool isTower);
};

#endif