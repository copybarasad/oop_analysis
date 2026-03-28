#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include "Level.h"
#include "Player.h"
#include <memory>
#include <vector>

class LevelManager {
public:
    LevelManager();
    
    void StartLevel(int levelNumber, GameField& field);
    void CompleteLevel(Player& player);
    void ApplyLevelTransition(Player& player);
    
    bool ShowUpgradeMenu(Player& player);
    
    int GetCurrentLevelNumber() const { return currentLevelNumber; }
    const Level* GetCurrentLevel() const { return currentLevel.get(); }
    
    void SetCurrentLevel(int levelNumber);
    
private:
    int currentLevelNumber;
    std::unique_ptr<Level> currentLevel;
    
    void removeHalfSpells(Player& player);
    void restorePlayerHealth(Player& player);
};

#endif

