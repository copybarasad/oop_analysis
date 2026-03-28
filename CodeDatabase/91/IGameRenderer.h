#ifndef RENDERERS_IGAMERENDERER_H
#define RENDERERS_IGAMERENDERER_H

class Game;

class IGameRenderer {
    public:
        virtual ~IGameRenderer() = default;

        virtual void render(const Game& game) = 0;
};

#endif