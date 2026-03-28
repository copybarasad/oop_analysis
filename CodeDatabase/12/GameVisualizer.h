#pragma once
#include "Game.h"

template <typename Renderer>
class GameVisualizer {
private:
    Renderer renderer; 
    Game& game;

public:
    explicit GameVisualizer(Game& g) : game(g) {}
    
    void update() {
        renderer.render(game);
    }
};
