#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "LevelConfig.h"
#include "GameMode.h"

class LevelManager {
private:
    int currentLevel;
    LevelConfig currentConfig;
    GameMode gameMode;

public:
    LevelManager(GameMode mode = GameMode::CUSTOM);
    
    int getCurrentLevel() const;
    const LevelConfig& getCurrentConfig() const;
    GameMode getGameMode() const;
    
    void setLevel(int level);
    LevelConfig buildNextLevelConfig();
    void advanceToNextLevel();
    
    bool hasNextLevel() const;
    int getMaxLevel() const;
};

#endif
