#pragma once
#include "renderer.h"
#include "game.h"

template <typename Renderer>
class GameView {
public:
    GameView(Game& game) : game_(game), renderer_() {}
    void update() {
        renderer_.render(game_);
    }
private:
    Game& game_;
    Renderer renderer_;
};