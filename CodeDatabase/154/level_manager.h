#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include "field.h"
#include "player.h"

class LevelManager {
public:
    explicit LevelManager(int startLevel = 1);
    
    int getCurrentLevel() const;
    
    void nextLevel();
    
    void initializeLevel(Field& field, Player* player);
    
    struct LevelParams {
        int fieldRows;
        int fieldCols;
        int enemyCount;
        int buildingCount;
        int enemyHealth;
        int enemyDamage;
        int buildingHealth;
    };
    
    LevelParams getCurrentLevelParams() const;
    
private:
    int currentLevel_;
    
    LevelParams getLevelParams(int level) const;
};

#endif
