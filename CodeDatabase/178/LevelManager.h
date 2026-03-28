#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include "GameController.h"
#include "GameConfig.h"
#include "PlayerProgression.h"

class LevelManager {
private:
    int currentLevel;
    bool levelCompleted;
    bool gameCompleted;
    bool healthUpgraded;
    bool meleeUpgraded;
    bool rangedUpgraded;

public:
    LevelManager();
    
    int getCurrentLevel() const { return currentLevel; }
    bool isLevelCompleted() const { return levelCompleted; }
    bool isGameCompleted() const { return gameCompleted; }
    
    void setCurrentLevel(int level) { currentLevel = level; }
    void initializeLevel(GameController& controller);
    void checkLevelCompletion(GameController& controller);
    void proceedToNextLevel(GameController& controller);
    
    void setUpgradeStates(bool health, bool melee, bool ranged) {
        healthUpgraded = health;
        meleeUpgraded = melee;
        rangedUpgraded = ranged;
    }
    
private:
    void setupLevel(GameController& controller, const LevelConfig& config);
    bool areAllEnemiesDefeated(const GameController& controller) const;
    bool areAllTowersDestroyed(const GameController& controller) const;
};

#endif