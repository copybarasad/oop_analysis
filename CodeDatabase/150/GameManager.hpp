#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "GameState.hpp"
#include "GameExceptions.hpp"
#include "EntityManager.hpp"
#include "SaveLoadManager.hpp"
#include "InputHandler.hpp"
#include "UpgradeSystem.hpp"
#include "GamePhase.hpp"
#include <string>
#include <memory>

class GameInterface;

class GameManager {
private:
    std::unique_ptr<GameState> currentState;
    GamePhase currentPhase;
    bool gameRunning;

    GameInterface* gameInterface;
    
    SaveLoadManager saveLoadManager;
    InputHandler inputHandler;
    UpgradeSystem upgradeSystem;
    
    void gameLoop();
    
    void processMainMenu();
    void processPlayerTurn();
    void processAlliesTurn();
    void processEnemiesTurn();
    void processBuildingsTurn();
    void processLevelTransition();
    void processUpgradePhase();
    void processGameOver();
    void processVictory();
    
    bool isLevelComplete() const;
    bool isGameOver() const;
    
public:
    GameManager(GameInterface* interface);
    void startGame();
    void saveGame();
    void loadGame();
    
    GamePhase getCurrentPhase() const { return currentPhase; }
    const GameState* getCurrentState() const { return currentState.get(); }
    const UpgradeSystem& getUpgradeSystem() const { return upgradeSystem; }
    void setCurrentPhase(GamePhase phase) { currentPhase = phase; }
    void setGameRunning(bool running) { gameRunning = running; }
    void resetGameState(int level = 1) { currentState = std::make_unique<GameState>(level); }
};

#endif