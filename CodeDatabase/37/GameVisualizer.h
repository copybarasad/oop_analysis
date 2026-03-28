#ifndef GAMEVISUALIZER_H
#define GAMEVISUALIZER_H

#include <memory>
#include "Renderer.h"

template<typename RendererType>
class GameVisualizer {
private:
    std::unique_ptr<RendererType> renderer;
    const Game& game;
    
public:
    GameVisualizer(const Game& gameRef) 
        : renderer(std::make_unique<RendererType>()), game(gameRef) {}
    
    void render() {
        renderer->render(game);
    }
    
    void showMessage(const std::string& message) {
        renderer->showMessage(message);
    }
    
    void setRenderer(std::unique_ptr<RendererType> newRenderer) {
        renderer = std::move(newRenderer);
    }
};

#endif