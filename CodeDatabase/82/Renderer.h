#ifndef RENDERER_H
#define RENDERER_H

#include "GameState.h"

class Renderer {
public:
    void renderGameState(const GameState& gameState);
    void renderPlayerStats(const Player& player);
    void renderGameOverScreen(const GameState& gameState);
    
private:
    void renderGameField(const GameState& gameState);
    char getCellSymbol(const GameState& gameState, int x, int y);
};

#endif