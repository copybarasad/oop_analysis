#pragma once
#include "IRenderer.h"
#include "Game.h"

class ConsoleRenderer : public IRenderer {
public:
    void Render(Game& game) override {
        game.RenderField();
        game.RenderStatus();
    }
};
