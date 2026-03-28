#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "GameState.h"
#include "ActionProcessor.h"
#include "ShopSystem.h"
#include "InputHandler.h"
#include <memory>
#include <iostream>

class InputHandler;
class Command;

class GameLogic {
private:
    GameState& gameState;
    ActionProcessor& actionProcessor;
    ShopSystem& shopSystem;
    bool gameRunning;
    bool playerTurn;

public:
    GameLogic(GameState& state, ActionProcessor& processor, ShopSystem& shop);
    void startGame();
    void stopGame();
    bool isGameRunning() const;

    // »«Ã≈Õ®ÕÕ¿ﬂ —»√Õ¿“”–¿: Û·‡Ì Ô‡‡ÏÂÚ char input
    bool processPlayerAction(InputHandler& inputHandler);

    void processEnemyTurn();
    bool isPlayerTurn() const;
    bool isPlayerAlive() const;
    bool checkPlayerVictory() const;
    bool isGameOver() const;
    GameState& getGameState();
    const GameState& getGameState() const;
    int getPlayerScore() const;
    int getPlayerHealth() const;
    int getPlayerMana() const;
    void reset();
};

#endif // GAMELOGIC_H