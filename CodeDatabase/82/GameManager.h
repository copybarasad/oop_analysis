#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "GameState.h"
#include "InputHandler.h"
#include "GameLogic.h"
#include "Renderer.h"
#include <memory>
#include <string>

class GameManager {
private:
    std::unique_ptr<GameState> gameState;
    InputHandler inputHandler;
    GameLogic gameLogic;
    Renderer renderer;
    bool isRunning;

    void startNewGame();
    void loadGame();
    void runGameLoop();
    void handlePlayerTurn();
    void checkGameOver();
    void saveGame();
    void promptForSaveOrLoad();
    void promptForNewGameOrLoad();

public:
    GameManager();
    void run();
};

#endif