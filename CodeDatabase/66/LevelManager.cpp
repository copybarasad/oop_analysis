#include "../include/LevelManager.h"
#include "../include/GameMode.h"

LevelManager::LevelManager(GameMode mode) 
    : currentLevel(1), currentConfig(1, mode), gameMode(mode) {}

int LevelManager::getCurrentLevel() const {
    return currentLevel;
}

const LevelConfig& LevelManager::getCurrentConfig() const {
    return currentConfig;
}

GameMode LevelManager::getGameMode() const {
    return gameMode;
}

void LevelManager::setLevel(int level) {
    currentLevel = level;
    currentConfig = LevelConfig(level, gameMode);
}

LevelConfig LevelManager::buildNextLevelConfig() {
    return LevelConfig(currentLevel + 1, gameMode);
}

void LevelManager::advanceToNextLevel() {
    currentLevel++;
    currentConfig = LevelConfig(currentLevel, gameMode);
}

bool LevelManager::hasNextLevel() const {
    return currentLevel < getMaxLevel();
}

int LevelManager::getMaxLevel() const {
    if (gameMode == GameMode::LEVELS) {
        return 3;
    }
    return 10;
}
