#ifndef CONSOLE_RENDERER_H
#define CONSOLE_RENDERER_H

#include "game_renderer.h"
#include <iostream>

class ConsoleRenderer : public GameRenderer {
public:
    void renderField(const Game& game) override;
    void renderPlayerStatus(const Game& game) override;
    void renderSpellList(const Game& game) override;
    void renderMessage(const std::string& message) override;
    void renderControls() override;
};

#endif
