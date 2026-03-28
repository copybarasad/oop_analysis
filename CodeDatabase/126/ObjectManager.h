#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include "../gamelogic/GameState.h"

class ObjectManager {
    GameState& gameState;
    
public:
    ObjectManager(GameState& state);
    
    void cleanupDeadObjects();
    void checkTraps();
};

#endif