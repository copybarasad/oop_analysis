#pragma once
#include "Game.h"

template <typename Renderer>
class GameView {
public:
    GameView()
        : renderer_()
    {}

    void Render(Game& game) {
        renderer_.Render(game);
    }

private:
    Renderer renderer_;
};
