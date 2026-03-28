#include "LevelManager.h"

LevelManager::LevelManager() : currentLevelIndex(0) {
    levels.push_back(LevelConfig(15, 15, 3, 2, 10, 1, 0));
    levels.push_back(LevelConfig(18, 18, 5, 4, 15, 2, 1));
    levels.push_back(LevelConfig(20, 20, 7, 6, 20, 3, 2));
    levels.push_back(LevelConfig(22, 22, 9, 8, 25, 4, 3));
    levels.push_back(LevelConfig(25, 25, 12, 10, 30, 5, 4));
}

LevelConfig LevelManager::getCurrentLevelConfig(int lvl) const {
    if(lvl > -1){
        if (lvl < levels.size()) {
            return levels[lvl];
        }
    }
    if (currentLevelIndex < levels.size()) {
        return levels[currentLevelIndex];
    }
    return levels.back();
}

LevelConfig LevelManager::getNextLevelConfig() const {
    if (currentLevelIndex + 1 < levels.size()) {
        return levels[currentLevelIndex + 1];
    }
    return levels.back();
}

void LevelManager::goToNextLevel() {
    if (currentLevelIndex < levels.size() - 1) {
        currentLevelIndex++;
    }
}

bool LevelManager::hasMoreLevels() const {
    return currentLevelIndex < levels.size() - 1;
}