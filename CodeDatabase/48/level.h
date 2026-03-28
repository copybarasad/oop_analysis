#ifndef LEVEL_H
#define LEVEL_H

#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include <vector>

class Level {
private:
    int levelNumber;
    GameField* gameField;
    Player* player;
    std::vector<Enemy> enemies;
    std::vector<EnemyTower> towers;
    bool levelCompleted;
    bool levelFailed;

public:
    Level(int number, GameField* field, Player* player);
    
    void initialize();
    void start();
    void processPlayerTurn(char input);
    void processEnemiesTurn();
    void processTowersTurn();
    void checkLevelCompletion();
    
    bool isCompleted() const;
    bool isFailed() const;
    bool isRunning() const;
    
    void displayLevelInfo() const;

private:
    void createEnemies();
    void createTowers();
    void handleAutoAttack(); 
    bool isWinConditionMet() const;
    bool isLoseConditionMet() const;
    
    // Система сохранения/загрузки
    void saveGame();
    void loadGame();
    void showSaveLoadMenu();
    void showSaveInfo();
    std::string getSaveSlotName();
};

#endif