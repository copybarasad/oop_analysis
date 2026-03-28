#ifndef CONSOLE_RENDERER_H
#define CONSOLE_RENDERER_H

#include "renderer.h"

class ConsoleRenderer : public Renderer {
public:
    void render(const Game& game) override;
    void renderMenu() override;
    void renderGameOver() override;
    void renderControls() override;
    void clearScreen();
    
    
private:
    void renderField(const Game& game);
    void renderPlayerInfo(const Game& game);
    void renderSpells(const Game& game);
};

#endif