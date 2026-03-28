#include "LevelManager.h"
#include "Game.h"
#include "GameConstants.h"
#include <algorithm>

int LevelManager::calculateEnemyCount(int level) {
    return GameConstants::ENEMY_INITIAL_COUNT + (level - 1) * 2;
}

int LevelManager::calculateBuildingCount(int level) {
    return GameConstants::BUILDING_INITIAL_COUNT + (level - 1);
}

int LevelManager::calculateTowerCount(int level) {
    return GameConstants::TOWER_INITIAL_COUNT + (level - 1);
}

int LevelManager::calculateFieldWidth(int level) {
    int maxSize = GameConstants::MAX_FIELD_SIZE;
    if (level == 1) {
        return std::min(15, maxSize);
    }
    if (level == 2) {
        return std::min(20, maxSize);
    }
    if (level == 3) {
        return std::min(25, maxSize);
    }
    int calculatedSize = 25;
    return std::min(calculatedSize, maxSize);
}

int LevelManager::calculateFieldHeight(int level) {
    return calculateFieldWidth(level);
}

int LevelManager::calculateEnemyHealth(int level) {
    (void) level;
    return GameConstants::ENEMY_BASE_HEALTH;
}

int LevelManager::calculateEnemyDamage(int level) {
    int baseDamage = (GameConstants::ENEMY_MIN_DAMAGE + GameConstants::ENEMY_MAX_DAMAGE) / 2;
    return baseDamage + (level - 1) * 2;
}

int LevelManager::calculateEnemyMinDamage(int level) {
    return GameConstants::ENEMY_MIN_DAMAGE + (level - 1) * 2;
}

int LevelManager::calculateEnemyMaxDamage(int level) {
    return GameConstants::ENEMY_MAX_DAMAGE + (level - 1) * 3;
}

int LevelManager::getScoreTarget(int level) {
    if (level == 1) {
        return 15 * GameConstants::SCORE_PER_KILL;
    }
    if (level == 2) {
        return 20 * GameConstants::SCORE_PER_KILL;
    }
    if (level == 3) {
        return 30 * GameConstants::SCORE_PER_KILL;
    }
    return 30 * GameConstants::SCORE_PER_KILL;
}

bool LevelManager::isLevelComplete(const Game &game) {
    int level = game.getCurrentLevel();
    int scoreTarget = getScoreTarget(level);
    return game.getPlayerScore() >= scoreTarget;
}

void LevelManager::initializeLevel(Game &game, int level) {
    int fieldWidth = calculateFieldWidth(level);
    int fieldHeight = calculateFieldHeight(level);

    game.setFieldSize(fieldWidth, fieldHeight);
    game.setCurrentLevel(level);

    game.clearEnemies();
    game.clearBuildings();
    game.clearTowers();
    game.clearTraps();
    game.clearAllies();

    game.initializeLevel(level);
}
