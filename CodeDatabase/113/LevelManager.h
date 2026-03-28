#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "GameState.h"
#include "Player.h"
#include "ObstacleGenerator.h"
#include <memory>

class LevelManager {
private:
    std::unique_ptr<ObstacleGenerator> m_obstacleGenerator;
    int m_currentLevel;

public:
    LevelManager();

    bool initializeLevel(GameState& gameState, int level);
    bool advanceToNextLevel(GameState& gameState, Player& player);
    int getCurrentLevel() const { return m_currentLevel; }
    void reset() { m_currentLevel = 1; }

private:
    void setupEnemiesForLevel(GameState& gameState, int level);
    void setupTowersForLevel(GameState& gameState, int level);
    void applyLevelBonuses(Player& player, int level);
};

#endif // LEVELMANAGER_H