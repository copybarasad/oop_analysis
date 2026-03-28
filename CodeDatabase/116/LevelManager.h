#pragma once
#include <vector>
#include <memory>

struct LevelConfig {
    int width;
    int height;
    int enemies;
    int slowingCells;
    int wallsCount;
    int fortressesCount;
    int lvl;
    
    LevelConfig(int w, int h, int e, int s, int wc, int fc, int lvl)
        : width(w), height(h), enemies(e), slowingCells(s), wallsCount(wc), fortressesCount(fc), lvl(lvl) {}
};

class LevelManager {
private:
    std::vector<LevelConfig> levels;
    int currentLevelIndex;
    
public:
    LevelManager();
    
    LevelConfig getCurrentLevelConfig(int lvl = -1) const;
    LevelConfig getNextLevelConfig() const;
    void goToNextLevel();
    bool hasMoreLevels() const;
    int getCurrentLevelNumber() const { return currentLevelIndex + 1; }
    int getTotalLevels() const { return levels.size(); }
    void setCurrentLevel(int level) {
        if (level >= 0 && level < levels.size()) {
            currentLevelIndex = level;
        }
    }
};