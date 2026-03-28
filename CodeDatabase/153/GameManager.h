#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "GameState.h"
#include "GameLogic.h"
#include "GameController.h"
#include "ConsoleRenderer.h"
#include "GameVisualizer.h"
#include "ConsoleInputHandler.h"
#include "ActionProcessor.h"
#include "ShopSystem.h"
#include "GameSaveSystem.h"
#include "SaveLoadException.h"
#include <memory>

class GameManager {
private:
    GameState gameState;
    ActionProcessor actionProcessor;
    ShopSystem shopSystem;
    GameSaveSystem saveSystem;
    GameLogic gameLogic;
    GameController<ConsoleInputHandler> controller;
    GameVisualizer<ConsoleRenderer> visualizer;

    void handleGameOver();
    void handleVictory();
    void showMainMenu();
    void startNewGame();
    void loadGame();
    void runGameLoop();

public:
    GameManager();
    void run();
};

#endif // GAMEMANAGER_H