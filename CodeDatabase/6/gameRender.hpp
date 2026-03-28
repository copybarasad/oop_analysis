#ifndef GAME_RENDERER
#define GAME_RENDERER

#include <memory>
#include <SFML/Graphics.hpp>

#include "../headers/gameManager.hpp"
#include "../gui_headers/gameState.hpp"
#include "../gui_headers/gameGUI.hpp"

template <typename RendererType>
class GameRenderer{
    private:
        std::unique_ptr<RendererType> renderer;

    public:
        GameRenderer(){
            this->renderer = std::make_unique<RendererType>();
        }

        void render(const GameState& state, GameManager* gameManager, int currentLevel, const KeyBindingsInfo& keyBindings){
            if (this->renderer && gameManager){
                renderer->render(state, gameManager, currentLevel, keyBindings);
            }
        }

        RendererType* getRenderer() const{ 
            return renderer.get(); 
        }
};

#endif