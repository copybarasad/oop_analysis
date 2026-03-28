#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "GameManager.h"
#include "GameEnums.h"
#include "GameSave.h"
#include "GameRenderer.h"
#include <memory>

class GameController {
private:
    std::unique_ptr<GameManager> gameManager;
    bool gameRunning;
    int currentLevel;
    int playerScore;
    GameSave gameSave;

    void initializeStartingSpell();

public:
    GameController();
    void processPlayerAction(Direction direction);
    void processAttack(Direction direction);
    void processCombatModeSwitch();
    void processSpellCast(int spellIndex, int targetX, int targetY);
    void processSpellPurchase();
    void updateGame();
    bool isGameRunning() const;
    bool isGameOver() const;
    
    void startNewGame();
    void nextLevel();
    bool isLevelComplete() const;

    int getCurrentLevel() const { return currentLevel; }
    int getPlayerScore() const { return playerScore; }
    const GameManager& getGameManager() const { return *gameManager; }

    bool saveGame();
    bool loadGame();
    void loadFromData(const GameSaveData& data);
};

#endif

