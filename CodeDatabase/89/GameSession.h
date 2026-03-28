#ifndef GAME_SESSION_H
#define GAME_SESSION_H

#include "GameController.h"
#include "GameVisualizer.h"
#include "InputHandler.h"
#include "Renderer.h"
#include <string>
#include <vector>
#include <memory>

class GameSession {
public:
    GameSession();
    void run();

private:
    using Controller = GameController<ConsoleInputHandler>;
    using Visualizer = GameVisualizer<ConsoleRenderer>;

    void showMainMenu();
    bool startNewGame();
    bool startLevel(int level);
    bool loadGame(const std::string& filename);
    void saveGame(const std::string& filename);
    void gameOverMenu();
    void nextLevel();
    void showUpgradeMenu();

    std::unique_ptr<Controller> gameController;
    std::unique_ptr<Visualizer> gameVisualizer;
    int currentLevel = 1;
    bool sessionRunning = true;

    GameSession(const GameSession&) = delete;
    GameSession& operator=(const GameSession&) = delete;
};

#endif