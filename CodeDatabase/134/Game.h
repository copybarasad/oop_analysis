#pragma once

#include "GameField.h"
#include "Player.h"
#include "Hand.h"

class Game {
public:
    Game();
    void Run();

    void SaveGame(const std::string& filename);
    void LoadGame(const std::string& filename);

private:
    void StartGame();
    void StartLevel();

    void PlayerTurn(bool& playerMovedThisTurn);
    void EnemyTurn();

    void HandlePlayerCommand(bool& playerMovedThisTurn);

    GameField field;
    Player player;
    Hand hand;

    bool levelStarted;
    bool levelActive;
    bool levelCompleted;
    bool isRunning;
    bool gameLoaded;
    bool gameOver;

    void CheckWinLoseConditions();
    void HandleGameOver();

};
