#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "GameState.h"
#include "InputHandler.h"
#include <vector>

class GameLogic {
public:
    bool processPlayerAction(GameState& gameState, GameAction action);
    void updateEnemies(GameState& gameState);
    bool checkGameOver(const GameState& gameState);
    bool processSpellCast(GameState& gameState, size_t spellIndex, const Position& target);
    
private:
    void handlePlayerMovement(GameState& gameState, const Position& direction);
    void handleRangedAttack(GameState& gameState);
    void handleEnemyMovement(GameState& gameState, Enemy& enemy, std::vector<Enemy>& allEnemies);
    bool isValidMovement(const GameState& gameState, const Position& position);
    bool handleSpellCast(GameState& gameState, size_t spellIndex);
    void handleEnemyDefeated(GameState& gameState);
};

#endif