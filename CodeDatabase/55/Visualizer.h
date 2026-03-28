#pragma once

#include <memory>

class Game;


template <typename RendererType>
class GameVisualizer {

public:
    GameVisualizer(Game& game, std::unique_ptr<RendererType> renderer)
        : game(game), renderer(std::move(renderer)) {}
    
    void renderMenu(){
        renderer->renderMenu();
    }

    void renderStatistics() {
        renderer->renderStatistics(game);
    }
    
    void renderControls() {
        renderer->renderControls();
    }
    
    void renderField() {
        renderer->renderField(game);
    }
    
    void clear() {
        renderer->clearScreen();
    }

private:
    Game& game;
    std::unique_ptr<RendererType> renderer;
};