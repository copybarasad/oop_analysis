#pragma once
#include "Game.h"
#include "console_renderer.h"
#include "color_vis.h"
#include <type_traits>
#include <memory>

template<typename RendererType>
class GameVisualization {
private:
    RendererType renderer;
    Game* game;

public:
    GameVisualization(Game* gameInstance) : game(gameInstance) {}

    void renderGame() {
        if (game) {
            renderer.render(*game);
        }
    }
    
    void renderMessage(const std::string& msg) {
        renderer.renderMessage(msg);
    }
    
    void showPrompt(const std::string& prompt) {
        renderer.showPrompt(prompt);
    }

    RendererType& getRenderer() { return renderer; }
    const RendererType& getRenderer() const { return renderer; }
};