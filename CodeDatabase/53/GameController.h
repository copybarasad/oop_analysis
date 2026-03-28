#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "Command.h"
#include <memory>
#include <iostream>
#include <vector>

class Game;

template<typename TInputHandler>
class GameController {
private:
    Game* game;
    std::unique_ptr<TInputHandler> inputHandler;
    bool isRunning;
    
public:
    explicit GameController(Game* gameInstance);
    ~GameController() = default;
    
    void runGameLoop();
    

    void showMainMenu();
    void showLoadMenu();
    void handleLoadException(const std::exception& e);
    void startNewGame();
    void processPlayerTurn();
    void displayGameState() const;
    void processEnemyTurns();
    void displayGameResult();
    void quit();
    void advanceToNextLevel();
    void removeHalfOfSpells();
    void showLevelUpMenu();
    void upgradeMeleeDamage();
    void upgradeRangedDamage();
    void upgradeAttackRange();
    
private:
    bool isGameOver() const;
    bool isVictory() const;
    bool isDefeat() const;
    void waitForContinue() const;
};

#endif