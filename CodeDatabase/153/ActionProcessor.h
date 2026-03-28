#ifndef ACTIONPROCESSOR_H
#define ACTIONPROCESSOR_H

#include "GameState.h"
#include "InputHandler.h"
#include "ShopSystem.h"    
#include "PhysicalWorldSystem.h" 


class ActionProcessor {
private: 
    PhysicalWorldSystem physicalWorldSystem; 

public:

    void processEnemyMoves(GameState& state);

    bool processPlayerMove(GameState& state, int newX, int newY);
};
    
#endif

