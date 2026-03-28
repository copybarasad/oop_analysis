#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "GameState.hpp"
#include "GameInterface.hpp"
#include "GamePhase.hpp"
#include "EntityManager.hpp"
#include <memory>

class InputHandler {
private:
    GameInterface* gameInterface;

public:
    InputHandler(GameInterface* interface);
    
    bool handleMovement(GameState& state, GamePhase& currentPhase, bool& gameRunning);
    
    bool handleSpellCast(GameState& state, int spellIndex);
    bool handleRangedAttack(GameState& state);
    bool handleBuySpell(GameState& state);
    
    bool movePlayer(GameState& state, Direction direction);
    bool castSpell(GameState& state, int spellIndex, int targetX, int targetY);
    bool playerRangedAttack(GameState& state, int targetX, int targetY);
};

#endif