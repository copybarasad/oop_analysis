#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "game_state.hpp"

class Renderer {
public:
    void renderGameState(const GameState& gameState) const;
    void renderMap(const GameState& gameState) const;
    void renderPlayerInfo(const Player& player) const;
    void renderSpellInfo(const Player& player) const;
    void renderGameStats(const GameState& gameState) const;
    
private:
    char getCellSymbol(const GameState& gameState, int x, int y) const;
};

#endif