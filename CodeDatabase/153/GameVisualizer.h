#ifndef GAMEVISUALIZER_H
#define GAMEVISUALIZER_H

#include "Renderer.h"
#include <memory>

template<typename RendererType>
class GameVisualizer {
private:
    std::unique_ptr<RendererType> renderer;

public:
    GameVisualizer();

    void render(const GameState& gameState) const;
    void showMessage(const std::string& message) const;
};

template<typename RendererType>
GameVisualizer<RendererType>::GameVisualizer()
    : renderer(std::make_unique<RendererType>()) {
}

template<typename RendererType>
void GameVisualizer<RendererType>::render(const GameState& gameState) const {
    renderer->render(gameState);
}

template<typename RendererType>
void GameVisualizer<RendererType>::showMessage(const std::string& message) const {
    renderer->showMessage(message);
}

#endif