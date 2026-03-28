#ifndef GAMEVISUALIZER_H
#define GAMEVISUALIZER_H

#include "Game.h"

// Шаблонный класс визуализатора 
template<typename RendererType>
class GameVisualizer {
private:
    RendererType renderer;
    const Game& game;
    
public:
    explicit GameVisualizer(const Game& g) : game(g) {}
    
    void forceRender() {
        renderer.render(game);
    }
    
    void render() {
        renderer.render(game);
    }
};

#endif