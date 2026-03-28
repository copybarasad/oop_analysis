#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "LevelController.h"
#include "TextSystem.h"
#include <memory>

class RenderSystem {
private:
    std::shared_ptr<TextSystem> textSystem;

public:
    RenderSystem(std::shared_ptr<TextSystem> textSystem);
    
    void renderGameState(const LevelController& level, int turnCount);
    void renderCommands();
    void renderMessage(const std::string& message);
    void renderGameOver(bool victory, const Player& player);

private:
    void renderGameField(const LevelController& level);
    char getCellSymbol(const LevelController& level, const Position& pos) const;
};

#endif