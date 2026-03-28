#pragma once

#include "i_game_visualizer.h"

template <typename Renderer>
class GameVisualizer final : public IGameVisualizer {
public:
    GameVisualizer() = default;

    void Render(const Game& game) override {
        renderer.Render(game);
    }

private:
    Renderer renderer;
};
