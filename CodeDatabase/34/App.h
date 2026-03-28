#ifndef APP_H
#define APP_H

#include "game/Player.h"
#include "game/input/KeyConfig.h" 
#include <memory>
#include <string>

namespace Game {

class Game;

class App {
public:
    App(int argc, char* argv[]);
    ~App();
    void run();

private:
    void mainLoop();
    void startNewGame();
    void loadGame();
    void runGameSession();
    void handlePlayerUpgrade();
    
    char showMainMenu();
    bool askToTryAgain();
    
    std::unique_ptr<Game> currentLevel;
    Player player;
    int level;
    int scoreToWin;
    const std::string saveFilename = "savegame.dat";
    std::string loggerType;
    
    KeyConfig keyConfig;
};

}

#endif