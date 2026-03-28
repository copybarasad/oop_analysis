#pragma once
#include <memory>
#include <iostream>

class Game;

template<typename RenderStrategy>
class GameRenderer {
private:
    RenderStrategy renderStrategy;
    Game* game;

public:
    explicit GameRenderer(Game* gameInstance) 
        : game(gameInstance), renderStrategy() {
        if (!gameInstance) {
            std::cerr << "Warning: GameRenderer initialized with null game pointer\n";
        }
    }
    
    explicit GameRenderer(Game* gameInstance, const RenderStrategy& strategy)
        : game(gameInstance), renderStrategy(strategy) {
        if (!gameInstance) {
            std::cerr << "Warning: GameRenderer initialized with null game pointer\n";
        }
    }

    void render() {
        if (game) {
            renderStrategy.render(*game);
        } else {
            std::cerr << "Error: Cannot render - game pointer is null\n";
        }
    }
    
    void onGameStateChanged() {
        render();
    }
    
    void forceRender() {
        render();
    }
    
    RenderStrategy& getRenderStrategy() { return renderStrategy; }
    const RenderStrategy& getRenderStrategy() const { return renderStrategy; }
    
    void setRenderStrategy(const RenderStrategy& newStrategy) {
        renderStrategy = newStrategy;
    }
};