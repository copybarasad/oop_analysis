#pragma once
#include "GameCycle.h"
#include <memory>

template <typename RendererT>
class GameVisualizer {
public:
    GameVisualizer() {
        renderer_ = std::make_unique<RendererT>();
    }

    void draw(const GameCycle& game) {
        renderer_->render(game);
    }

private:
    std::unique_ptr<RendererT> renderer_;
};
