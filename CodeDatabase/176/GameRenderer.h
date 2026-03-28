#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include "GameManager.h"
#include "GameEnums.h"
#include <string>

class GameRenderer {
public:
    GameRenderer() = default;
    
    void render(const GameManager& gameManager, int currentLevel) const;

    std::string renderPlayerInfo(const Player& player, int currentLevel) const;
    std::string renderSpellInfo(const Player& player) const;
    std::string renderGameField(const GameField& field, const std::pair<int, int>&playerPos, const GameManager& gameManager) const;
    std::string renderLegend() const;
    
private:
    char getCellSymbol(const GameCell& cell, 
                      int x, int y,
                      const std::pair<int, int>& playerPos,
                      const GameManager& gameManager) const;
};

#endif
