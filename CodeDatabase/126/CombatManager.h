#ifndef COMBATMANAGER_H
#define COMBATMANAGER_H

#include "../gamelogic/GameState.h"
#include "../enums/Enums.h"

class CombatManager {
    GameState& gameState;
    
public:
    CombatManager(GameState& state);
    
    void performRangedAttack(Direction dir);
    void processEnemyTurns();
    void movePlayer(Direction dir);
};

#endif