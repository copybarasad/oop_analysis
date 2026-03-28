#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Enemy.h"
#include "GameField.h"
#include "Direction.h"
#include <vector>
#include <iostream>

class Game {
private:
    Player player;
    std::vector<Enemy> enemies;
    GameField field;
    bool gameRunning;
    bool combatJustHappened;

public:
    Game();
    
    // Core game methods
    void displayGameField() const;
    void displayStatus() const;
    void processInput(char input); // Kept for backward compatibility if needed
    
    // New methods for direct command access
    void movePlayer(Direction dir);
    void switchPlayerMode();
    void quitGame();
    
    void processEnemyTurns();
    void checkCombat();
    void checkGameState();
    
    bool isRunning() const { return gameRunning; }
    
    // Helper methods for renderer
    const Player& getPlayer() const { return player; }
    const std::vector<Enemy>& getEnemies() const { return enemies; }
    const GameField& getField() const { return field; }
    
    void showControls() const;
    
private:
    // Helper methods for movement logic
    bool isValidMove(const Position& newPos) const;
    bool isEnemyAtPosition(const Position& pos) const;
    void performPlayerMove(const Position& newPos);
};

#endif