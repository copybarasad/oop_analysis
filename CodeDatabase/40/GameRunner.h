#ifndef GAMERUNNER_H
#define GAMERUNNER_H
#include "Game.h"
#include <string>

class GameRunner{
private:
    Game currentGame;
    

public:
    bool gameRunning = false;
    void run();
    void startNewGame();
    void loadGame();
    void saveGame();
    void showMenu();
    void handleGameEnd();
};

#endif