#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "../gamelogic/GameState.h"

class LevelManager {
    GameState& gameState;
    
public:
    LevelManager(GameState& state);
    
    void startLevel();
    void nextLevel();
    
private:
    void initializeEnemies();
    void initializeTowers();
};

#endif