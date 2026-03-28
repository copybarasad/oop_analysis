#ifndef RENDERERS_CONSOLEGAMERENDERER_H
#define RENDERERS_CONSOLEGAMERENDERER_H

#include "IGameRenderer.h"

class ConsoleGameRenderer : public IGameRenderer {
    public:
        ConsoleGameRenderer() = default;
        ~ConsoleGameRenderer() override = default;

        void render(const Game& game) override;
};

#endif