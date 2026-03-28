#ifndef GAMESESSION_H
#define GAMESESSION_H

#include "GameState.h"
#include "Player.h"
#include "LevelManager.h"
#include "SpellManager.h"
#include "TurnManager.h"
#include <string>

class GameSession {
private:
    GameState m_gameState;
    Player m_player;
    LevelManager m_levelManager;
    SpellManager m_spellManager;
    TurnManager m_turnManager;
    bool m_isRunning;
    std::string m_saveFileName;

public:
    GameSession();

    bool initializeNewGame();
    bool loadGame();
    bool saveGame();
    void processTurn();
    void processEnemyTurns();

    bool isRunning() const { return m_isRunning; }
    bool isVictory() const;
    bool isGameOver() const;

    GameState& getGameState() { return m_gameState; }
    Player& getPlayer() { return m_player; }
    const GameState& getGameState() const { return m_gameState; }
    const Player& getPlayer() const { return m_player; }

    void stop() { m_isRunning = false; }
    void advanceToNextLevel();

private:
    void setupPlayerReferences();
};

#endif // GAMESESSION_H