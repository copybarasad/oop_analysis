#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "game.h"

template <typename Renderer>
class GameVisualizer {
private:
    Renderer renderer; 

public:
    void draw(game& g) {
        renderer.render(g);
    }
};

#endif
