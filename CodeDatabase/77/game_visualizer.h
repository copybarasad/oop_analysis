#include "game.h"

template <typename RendererT>
class GameVisualizer {
    RendererT renderer;
public:
    void update(const Game& game) {
        // system("clear"); 
        renderer.render(game);
    }
};