#ifndef RENDERER_H
#define RENDERER_H

class Game;

class IRenderer {
public:
    virtual ~IRenderer() = default;
    virtual void render(Game& game) = 0;
};

#endif
