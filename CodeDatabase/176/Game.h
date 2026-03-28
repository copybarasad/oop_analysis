#ifndef GAME_H
#define GAME_H

#include "GameController.h"
#include "TemplateGameManager.h"
#include "InputHandler.h"
#include "GameRenderer.h"
#include <memory>

class Game {
private:
    bool gameRunning;

    void showMainMenu();
    void startNewGame();
    void loadGame();

public:
    Game();
    void run();
};

#endif
