#ifndef GAME_H
#define GAME_H

#include "GameEngine.h"
#include "SaveManager.h"
#include "GameController.h"
#include "GameVisualizer.h"
#include <string>
#include <memory>
#include "ConsoleInputHandler.h"
#include "ConsoleRenderer.h"



class Game {
private:
    std::unique_ptr<GameEngine> currentGame;
    std::unique_ptr<SaveManager> saveManager;
    std::unique_ptr<GameController<ConsoleInputHandler>> gameController;
    std::unique_ptr<GameVisualizer<ConsoleRenderer>> gameVisualizer;
    bool isRunning;
    int currentLevel;
    bool justLoaded;
    
    void showMainMenu();
    void processMainMenuInput();
    bool processGameInput();
    bool playerTurn();
    void enemiesTurn();
    bool checkLevelComplete();
    bool checkGameOver();
    void showLevelResult(bool won);
    void runLevel();
    void setupLevel();
    void askForRestart();
    
public:
    Game();
    void start();
    void startNewGame();
    std::vector<Position> getEnemyPositions() const;
    std::vector<int> getEnemyHealths() const;
    std::vector<int> getEnemyDamages() const;
    bool isEnemyTower(int index) const;
    int getEnemyAttackRange(int index) const;
};

#endif