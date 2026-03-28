#pragma once

template<typename RendererImpl>
class GameRenderer {
public:
    GameRenderer() : impl_() {}

    void Render(GameWorld& world, GameLogic& logic) {
        impl_.Render(world, logic);
    }

private:
    RendererImpl impl_;
};
