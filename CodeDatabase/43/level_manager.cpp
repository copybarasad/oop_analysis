#include "level_manager.h"

LevelManager::LevelManager() : currentLevel(1) {}

LevelConfig LevelManager::getCurrentLevelConfig() const {
    LevelConfig config;
    config.levelNumber = currentLevel;
    config.enemyCount = calculateEnemyCount();
    config.enemyHealth = calculateEnemyHealth();
    config.enemyDamage = calculateEnemyDamage();
    return config;
}

void LevelManager::goToNextLevel() {
    currentLevel++;
}

void LevelManager::setCurrentLevel(int level) {
    if (level < 1) level = 1;
    currentLevel = level;
}

int LevelManager::getCurrentLevel() const {
    return currentLevel;
}

int LevelManager::calculateEnemyHealth() const {
    return 50 + (currentLevel - 1) * 15;
}

int LevelManager::calculateEnemyDamage() const {
    return 8 + (currentLevel - 1) * 3;
}

int LevelManager::calculateEnemyCount() const {
    return 3 + (currentLevel - 1) / 2;
}