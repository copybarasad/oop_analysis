#pragma once

namespace game {
    class Game;

    template<class RendererT>
    class Visualizer {
        RendererT renderer_;

        public:

        void draw(const Game& game) { renderer_.draw(game); }
        void clear() const { renderer_.clear(); }

        RendererT& getRenderer() { return renderer_; }
    };
}