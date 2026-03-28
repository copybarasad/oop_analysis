#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include "renderer.h"
#include <memory>

template <typename Renderer>
class GameView {
public:
    GameView() { renderer = std::make_unique<Renderer>(); }
    void render(Game& game) { renderer->render(game); }
private:
    std::unique_ptr<IRenderer> renderer;
};

#endif
