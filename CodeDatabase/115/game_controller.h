#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include <memory>
#include <string>

class GameState;
class Logger;
class ConsoleRenderer;
class InputHandler;
template <typename T> class Visualizer;
template <typename T> class GameEngine;

class GameController {
private:
    std::unique_ptr<GameState> game_state;
    std::shared_ptr<Logger> logger;
    
    std::unique_ptr<Visualizer<ConsoleRenderer>> visualizer;
    std::unique_ptr<GameEngine<InputHandler>> engine;

    int current_level;
    bool is_game_running;

    int parseLogMode(int argc, char* argv[]);
    
    void rebuildArchitecture();
    
    void createNewLevel(int level, bool is_new_game);
    void showMainMenu();
    void startNewGame();
    bool loadGame();
    void processGameLoop();
    void showLevelComplete();
    void showPlayerUpgrade();
    void showGameOver();
    bool isLevelComplete() const;

public:
    GameController(int argc, char* argv[]);
    ~GameController();

    int run();
};

#endif