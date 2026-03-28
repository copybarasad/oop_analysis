#ifndef CONSOLERENDERER_H
#define CONSOLERENDERER_H

#include "IRenderer.h"

class ConsoleRenderer : public IRenderer {
public:
    ConsoleRenderer() = default;
    void render(const GameModel &m) override;
};

#endif // CONSOLERENDERER_H
