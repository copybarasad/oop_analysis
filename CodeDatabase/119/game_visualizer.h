#include "game.h"

template <typename RendererT>
class GameVisualizer {
    RendererT renderer;
public:
    void update(const Game& game) {
        renderer.render(game);
    }
};