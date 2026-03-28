#ifndef GAME_H
#define GAME_H

#include "GameField.h"
#include "GameController.h"
#include <memory>

class Game {
private:
    std::unique_ptr<GameField> gameField;
    std::unique_ptr<GameController> gameController;
    bool isRunning;

public:
    Game();
    void start();
    void startNewGame();  
    void exit();
    void showMainMenu();
    void handleGameOver();
    bool shouldRestart() const;

private:
    void loadSavedGame(); 
};

#endif