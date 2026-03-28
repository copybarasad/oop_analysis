#pragma once

#include "Renderer.hpp"
#include <memory>

class GameWorld;

template<typename RendererType>
class GameRenderer {
public:
    GameRenderer() : renderer_(std::make_unique<RendererType>()) {}

    void renderGameState(const GameWorld& world) const {
        renderer_->render(world);
    }

private:
    std::unique_ptr<RendererType> renderer_;
};
