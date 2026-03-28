#pragma once
#include "GameController.h"
#include "SaveMenu.h"
#include "SaveData.h"
#include "Renderer.h"
#include "InputHandler.h"
#include <memory>

enum class GamePhase {
    MAIN_MENU,
    IN_GAME,
    GAME_OVER,
    SAVE_MENU,
    LOAD_MENU,
    QUIT
};

class GameManager {
private:
    std::unique_ptr<GameController> gameController;
    SaveMenu saveMenu;
    GamePhase currentPhase;
    bool gameRunning;
    
    void showMainMenu();
    void startNewGame();
    void handleSaveGame();
    bool handleLoadGame();
    void showGameOverMenu();
    void restartGame();
    void cleanup();

public:
    GameManager();
    void run();
};
