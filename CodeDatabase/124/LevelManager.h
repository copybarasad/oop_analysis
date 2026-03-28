#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "gamefield.h"
#include "player.h"
#include "enemy.h"
#include "EnemyManager.h"
#include <iostream>

class LevelManager {
private:
    int currentLevel;
    int totalLevels;

public:
    LevelManager();
    
    void setLevel(int level);
    int getCurrentLevel() const;
    int getTotalLevels() const;
    
    void initializeLevel(GameField& field, Player& player, Enemy& enemy, EnemyManager& enemyManager);
    void levelUp(Player& player, EnemyManager& enemyManager);  
    bool shouldLevelUp(const EnemyManager& enemyManager) const;
    void showLevelStartMessage(const GameField& field, const EnemyManager& enemyManager) const;
    void showLevelCompleteMessage(const Player& player, const EnemyManager& enemyManager) const;
};

#endif