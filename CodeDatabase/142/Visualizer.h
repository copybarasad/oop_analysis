#pragma once

#include "Game.h"

template <typename RenderPolicy>
class GameVisualizer {
public:
    explicit GameVisualizer(Game& gameRef) : game(gameRef) {}

    void refresh() {
        renderer.draw(game);
    }

private:
    Game& game;
    RenderPolicy renderer;
};

