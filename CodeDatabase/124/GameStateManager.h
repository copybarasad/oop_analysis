#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

#include "player.h"
#include "EnemyManager.h"
#include "gamefield.h"
#include "LevelManager.h"
#include "enemy.h"   
#include <iostream>
#include <functional>

class GameStateManager {
private:
    bool gameRunning;
    bool levelCompleted;

public:
    GameStateManager();
    
    bool isGameRunning() const;
    bool isLevelCompleted() const;
    void setGameRunning(bool running);
    void setLevelCompleted(bool completed);
    
    bool checkGameOver(const Player& player) const;
    bool checkLevelComplete(const EnemyManager& enemyManager) const;
    
    // ИЗМЕНЯЕМ: добавляем параметр restartCallback
    void handleGameOver(Player& player, EnemyManager& enemyManager, 
                       GameField& field, LevelManager& levelManager, 
                       Enemy& enemy, std::function<void()> restartCallback = nullptr);   
};

#endif