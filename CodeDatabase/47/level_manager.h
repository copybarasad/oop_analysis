#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include "game_state.h"

class LevelManager {
public:
    static void setupLevel(GameState& gameState, int level);
    static int calculateLevelWidth(int level);
    static int calculateLevelHeight(int level);
    static int calculateEnemyHealth(int level);
    static int calculateEnemyDamage(int level);
    static int calculateBuildingHealth(int level);
    static int calculateTowerHealth(int level);
    static int calculateEnemyCount(int level);
};

#endif