#pragma once
#include <memory>
#include <string>
#include "Field.h"
#include "Player.h"
#include "EventSystem.h"

class GameState;

enum class GameStatus {
    RUNNING,
    PLAYER_WON,
    PLAYER_LOST,
    LEVEL_COMPLETE
};

class Game {
private:
    std::unique_ptr<GameState> state_;
    GameStatus status_;
    int currentLevel_;
    bool isPaused_;

public:
    Game();
    ~Game();

    void run();
    void startNewGame();
    bool loadGame(const std::string& filename);
    bool saveGame(const std::string& filename) const;

    bool movePlayer(int dx, int dy);
    bool useSpell(int spellIndex, int targetX, int targetY);
    void switchAttackMode();
    void endTurn();

    GameStatus getStatus() const { return status_; }
    int getCurrentLevel() const { return currentLevel_; }
    const GameState* getState() const;
    GameState* getState();

    void advanceToNextLevel();
    void applyUpgrade(const std::string& upgradeType);
    
    void notifyEvent(const GameEvent& event) {
        EventSystem::getInstance().notify(event);
    }

private:
    void initializeLevel(int level);
    void processPlayerTurn();
    void processAlliesTurn();
    void processEnemiesTurn();
    void processStructuresTurn();
    void checkWinConditions();
    void handleLevelCompletion();
};