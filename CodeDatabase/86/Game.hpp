#pragma once
#include "GameContext.hpp"
#include "GameWorld.hpp"
#include "TurnManager.hpp"
#include <memory>
#include <string>

class Game {
    GameContext ctx_;
    std::unique_ptr<GameWorld> world_;
    
    std::unique_ptr<Sleeve> carriedSleeve_ = nullptr;

    int currentLevel_ = 1;
    int playerMaxHp_ = 100; 
    int savedDamage_ = 25;
    int savedTurnCount_ = 1;

public:
    void run(); 

private:


    void runLevel();
    int getInput();
    void startNewGame();

    void createNewLevel(int lvl);
    
    bool loopLevel(); 
    
    void saveGame();
    void loadGame();
    void upgradePlayer();
};
