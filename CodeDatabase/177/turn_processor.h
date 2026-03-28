#ifndef TURN_PROCESSOR_H
#define TURN_PROCESSOR_H

#include "player.h"
#include "enemy_manager.h"
#include "building_manager.h"
#include "combat_manager.h"
#include "tower.h"
#include <vector>

class TurnProcessor {
private:
    Player& player;
    EnemyManager& enemyManager;
    BuildingManager& buildingManager;
    CombatManager& combatManager;
    bool& gameRunning;
    std::vector<Tower>& towers;
    
    void processBuildingPhase();
    void processEnemyPhase();
    void processCombatPhase();
    void processCleanupPhase();
    void checkGameEndCondition();
    
public:
    TurnProcessor(Player& player, EnemyManager& enemyManager,
                 BuildingManager& buildingManager, CombatManager& combatManager,
                 bool& gameRunning, std::vector<Tower>& towers);
    
    void processGameTurn();
    void processCombatModeSwitch();
};

#endif