#pragma once

template<typename RendererType>
class GameVisualization {
private:
    RendererType renderer;
    
public:
    void update(const Game& game) {
        renderer.render(game);
    }
    void showMessage(const std::string& message) {
        renderer.showMessage(message);
    }
    
    RendererType& getRenderer() { return renderer; }
};