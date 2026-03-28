#pragma once
#include "Renderer.h"
#include "Game.h"

template<typename RendererType>
class GameVisualizer {
private:
    RendererType renderer;
    Game& game;
    
public:
    GameVisualizer(Game& gameRef) : game(gameRef) {}
    
    void render() {
        if (auto state = game.getState()) {
            renderer.render(*state);
        }
    }
    
    void update() {
        render();
    }
    
    void clear() {
        renderer.clear();
    }
};