#pragma once

template <typename RendererT>
class Visualizer {
private:
    RendererT renderer;

public:
    template <typename GameT>
    void draw(const GameT& game) {
        renderer.render(game);
    }
};