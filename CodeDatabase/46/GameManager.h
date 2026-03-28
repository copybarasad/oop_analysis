#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Player.h"
#include "GameField.h"
#include "Enemy.h"
#include <vector>

class GameManager {
private:
    Player player;
    GameField field;
    std::vector<Enemy> enemies;
    bool gameRunning;

public:
    GameManager();
    
    void startGame();
    void processTurn();
    void displayGameState() const;
    bool isGameRunning() const;
    
private:
    void initializeGame();
    void processPlayerTurn();
    void processEnemiesTurn();
    void checkGameOver();
    void displayField() const;
    void performAttack();
};

#endif