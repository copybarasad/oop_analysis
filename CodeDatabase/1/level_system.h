#pragma once
#include "field.h"
#include "vampire_hunter.h"
#include "dracula.h"

struct LevelConfig {
    int levelNumber;
    int fieldWidth;
    int fieldHeight;
    int draculaHealth;
    int towersCount;
    bool hasDraculaTomb;  //  ГРОБНИЦА ДРАКУЛЫ (только уровень 6)
};

class LevelSystem {
private:
    int currentLevel_;
    int maxLevels_;
    std::vector<LevelConfig> levelConfigs_;

public:
    LevelSystem();
    
    LevelConfig getCurrentLevelConfig() const;
    LevelConfig getLevelConfig(int level) const;
    void advanceToNextLevel();
    bool hasMoreLevels() const;
    bool isFinalLevel() const;
    
    int getCurrentLevel() const { return currentLevel_; }
    int getMaxLevels() const { return maxLevels_; }
    
    void applyLevelConfig(Field& field, VampireHunter& hunter, Dracula& dracula) const;
    Position getDraculaTombPosition(const Field& field) const;

private:
    void initializeLevelConfigs();
};