#ifndef GAME_H
#define GAME_H

#include "GameSession.h"
#include "ProgressionSystem.h"
#include "SaveSystem.h"
#include "InputHandler.h"
#include <iostream>

enum class GameState {
    MAIN_MENU,
    PLAYING,
    LEVEL_TRANSITION,
    GAME_OVER,
    VICTORY
};

class Game {
private:
    GameSession gameSession;
    ProgressionSystem progressionSystem;
    GameState currentState;
    bool running;

public:
    Game();
    ~Game() = default;
    
    void run();
    void setState(GameState newState);
    void stop();
    
    GameSession& getGameSession() { return gameSession; }
    GameState getCurrentState() const { return currentState; }
    
private:
    void showMainMenu();
    void handleMainMenuInput();
    void startNewGame();
    void loadGame();
    void showGameOverMenu();
    void showVictoryScreen();
    void handleGameOverInput();
    void handleVictoryInput();
    void cleanup();
};

#endif