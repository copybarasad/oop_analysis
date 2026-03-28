#pragma once
#include "game.h"
#include <iostream>
#include <type_traits>
#include <functional>

template<typename RendererType>
class GameVisualization {
private:
    RendererType renderer;
    Game* game;

    static_assert(
        std::is_same_v<decltype(std::declval<RendererType>().render(std::declval<Game&>())), void>,
        "RendererType must have method: void render(Game&)"
    );
    
    static_assert(
        std::is_same_v<decltype(std::declval<RendererType>().renderMessage(std::string{})), void>,
        "RendererType must have method: void renderMessage(string)"
    );

    static_assert(
        std::is_same_v<decltype(std::declval<RendererType>().showPrompt(std::string{})), void>,
        "RendererType must have method: void showPrompt(string)"
    );

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