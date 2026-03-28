#include "../gui_headers/gameRender.hpp"
template <typename RendererType>

GameRenderer(){
    this->renderer = std::make_unique<RendererType>();
}

void render(sf::RenderWindow& window, const GameState& state, GameManager* gameManager, int currentLevel, const KeyBindingsInfo& keyBindings){
    if (this->renderer && gameManager){
        renderer->render(window, state, gameManager, currentLevel, keyBindings);
    }
}

RendererType* getRenderer() const{ 
    return renderer.get(); 
}