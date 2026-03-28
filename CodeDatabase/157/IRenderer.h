#ifndef IRENDERER_H
#define IRENDERER_H

class GameModel;

class IRenderer {
public:
    virtual ~IRenderer() = default;
    virtual void render(const GameModel &m) = 0;
};

#endif // IRENDERER_H
