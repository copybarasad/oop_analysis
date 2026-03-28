#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "Level.h"
#include <vector>

class LevelManager {
private:
    std::vector<Level> levels;
    int currentLevelIndex;

public:
    LevelManager();

    void InitializeLevels();
    Level GetCurrentLevel() const;
    bool HasNextLevel() const;
    void AdvanceToNextLevel();
    void ResetToFirstLevel();
    int GetTotalLevels() const;
    int GetCurrentLevelNumber() const;
};

#endif