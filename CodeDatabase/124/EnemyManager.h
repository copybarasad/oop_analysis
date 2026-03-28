#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "enemy.h"
#include "gamefield.h"
#include "player.h"
#include <vector>

class EnemyManager {
private:
    int enemiesDefeated;
    int totalEnemiesSpawned;
    int currentLevel;
    const int max_enemies_to_win = 2;
    const int total_levels = 3;

public:
    EnemyManager();
    
    void updateEnemy(Enemy& enemy, GameField& field, Player& player);
    void spawnNewEnemy(Enemy& enemy, GameField& field, const Player& player);
    bool canSpawnEnemy() const;
    bool isGameWon() const;
    bool isLevelComplete() const;
    int getEnemiesDefeated() const;
    void setLevel(int level);
     
    void setEnemiesDefeated(int count);
    
    int getCurrentLevel() const;
    int getTotalLevels() const;
    void resetForNewLevel();
    void reset();
    void placeTowerOnEmptyCell(GameField& field);
     
private:
    std::vector<std::pair<int, int>> findSpawnPositions(const GameField& field, int playerX, int playerY);
};
#endif