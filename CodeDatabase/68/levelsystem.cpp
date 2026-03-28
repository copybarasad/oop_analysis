#include "levelsystem.h"

int LevelSystem::calculateLevelSize(int level) const {
    return 10 + level;  // Level 1: 11x11, Level 2: 12x12, etc.
}

int LevelSystem::calculateTowerCount(int level) const {
    return level * 2;   // Level 1: 2 towers, Level 2: 4 towers, etc.
}

int LevelSystem::calculateEnemyStrength(int level) const {
    return 50 + (level * 10);  // Increasing difficulty
}