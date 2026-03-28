#ifndef LEVEL_MANAGER_HPP
#define LEVEL_MANAGER_HPP

#include "game/entity_manager.hpp"

class LevelManager {
private:
    int currentLevel;
    
    void setupMap(eManager& manager, int w, int h);
    void setupPlayer(eManager& manager, int level);
    void setupEnemies(eManager& manager, int level);
    void setupTower(eManager& manager, int level);
    Position getRandomFreePosition(const eManager& manager);

public:
    LevelManager();
    
    void createLevel(eManager& manager, int level);
    void nextLevel();
    int getCurrentLevel();
    
};

#endif