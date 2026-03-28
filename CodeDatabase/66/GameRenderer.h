#ifndef GAMERENDERER_H
#define GAMERENDERER_H

#include "Position.h"
#include "CellType.h"
#include "Player.h"
#include "SpellHand.h"
#include "EnhancementBuffer.h"
#include "KeyBindingConfig.h"
#include <string>

class GameRenderer {
public:
    void renderTurnInfo(const Player& player, int turnNumber, bool isSlowed,
                       int lastDamageDealt, int lastEnemiesKilled, 
                       const SpellHand& spellHand, const EnhancementBuffer& buffer) const;
    void renderField(int width, int height, 
                    char (*getCellSymbol)(int x, int y, void* context),
                    void* context) const;
    void renderMessage(const std::string& message) const;
    void renderGameOver(int score, int turns) const;
    void renderVictory(int score, int turns) const;
    void renderWelcome() const;
};

#endif
