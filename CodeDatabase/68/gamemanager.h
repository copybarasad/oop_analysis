#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "gamefield.h"
#include "player.h"
#include "playerhand.h"
#include "levelsystem.h"
#include "savesystem.h"
#include "movingenemy.h"
#include "allyunit.h"
#include "enemybase.h"
#include <vector>
#include <memory>
#include <iostream>

class SaveSystem;

class GameManager {
private:
    std::unique_ptr<GameField> currentLevel;
    std::unique_ptr<Player> player;
    std::unique_ptr<SaveSystem> saveSystem;
    std::unique_ptr<LevelSystem> levelSystem;
    std::vector<MovingEnemy> movingEnemies;
    std::vector<AllyUnit> allies;
    std::vector<EnemyBase> enemyBases;
    int currentLevelNumber;
    bool gameRunning;

    friend class SaveSystem;

    void initializeFirstLevel();
    
public:
    GameManager();
    
    // Main game loop methods
    void startGame();
    void mainGameLoop();
    void processPlayerTurn();
    void processAllyTurns();
    void processEnemyBaseTurns();
    void processEnemyTurns();
    void processTowerTurns();
    
    // Game state methods
    bool checkLevelComplete() const;
    bool checkGameOver() const;
    void handleGameOver();
    void advanceToNextLevel();

    Player* getPlayer() const {
        return player.get();
    }

    int getCurrentLevel() const {
        return currentLevelNumber;
    }

    GameField* getCurrentLevelField() const {
        return currentLevel.get();
    }
    
    // Save/load interface
    void saveGame(const std::string& filename);
    void loadGame(const std::string& filename);
    
    // Utility methods
    void displayGameState() const;
    void handlePlayerInput();
    void cleanupCurrentLevel();
};

#endif