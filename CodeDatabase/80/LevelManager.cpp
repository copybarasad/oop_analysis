#include "LevelManager.h"

LevelManager::LevelManager() : currentLevelIndex(0) {
    InitializeLevels();
}

void LevelManager::InitializeLevels() {
    levels.clear();

    levels.push_back(Level(1, 15, 15, 30, 50, 10, 10, "The Beginning"));

    levels.push_back(Level(2, 20, 20, 40, 70, 15, 15, "Deeper Dungeon"));

    levels.push_back(Level(3, 25, 25, 50, 100, 20, 20, "Final Challenge"));
}

Level LevelManager::GetCurrentLevel() const {
    if (currentLevelIndex < levels.size()) {
        return levels[currentLevelIndex];
    }
    return levels.back();
}

bool LevelManager::HasNextLevel() const {
    return currentLevelIndex + 1 < levels.size();
}

void LevelManager::AdvanceToNextLevel() {
    if (HasNextLevel()) {
        currentLevelIndex++;
    }
}

void LevelManager::ResetToFirstLevel() {
    currentLevelIndex = 0;
}

int LevelManager::GetTotalLevels() const {
    return levels.size();
}

int LevelManager::GetCurrentLevelNumber() const {
    return currentLevelIndex + 1;
}