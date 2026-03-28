#ifndef CONSOLE_RENDERER_H
#define CONSOLE_RENDERER_H

#include "renderer.h"
class Game;

class ConsoleRenderer : public IRenderer {
public:
    void render(Game& game) override;
};

#endif
