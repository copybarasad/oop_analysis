#ifndef GAME_SESSION_H
#define GAME_SESSION_H

#include "GameController.h"
#include "LevelManager.h"
#include "SaveSystem.h"
#include "Logger.h"
#include "GameManager.h"
#include "RenderManager.h"
#include "ConsoleInputStrategy.h"
#include "ConsoleRenderStrategy.h"

class Game;

class GameSession {
private:
    std::unique_ptr<GameController> controller;
    std::unique_ptr<LevelManager> levelManager;
    std::unique_ptr<GameManager<ConsoleInputStrategy>> gameManager;
    std::unique_ptr<RenderManager<ConsoleRenderStrategy>> renderManager;
    bool sessionActive;

public:
    GameSession();
    
    void run(Game& game);
    void startNewGame();
    void cleanup();
    
    GameController* getController() const { return controller.get(); }
    LevelManager* getLevelManager() const { return levelManager.get(); }
    
    void setController(std::unique_ptr<GameController> newController);
    void setLevelManager(std::unique_ptr<LevelManager> newLevelManager);
    
    bool saveGame();
    bool loadGame();
    bool canSave() const { return controller && levelManager; }
    
private:
    void checkGameState(Game& game);
    void initializeManagers();
};

#endif