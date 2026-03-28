#pragma once
#include "Renderer.h"
#include <memory>

class Game;

template<typename RendererType>
class GameVisualizer {
private:
    std::unique_ptr<RendererType> renderer;
    Game* game;  

public:
    GameVisualizer(Game* gameInstance) : renderer(std::make_unique<RendererType>()), game(gameInstance) { }

    void renderGame() {
        if (!game || !renderer) return;
        
        auto& gameState = game->getGameState(); 
        
        renderer->clearScreen();
        std::cout << Colors::CYAN << "============= LEVEL " << gameState.getLevel() << " =============" << Colors::RESET << std::endl;
        renderer->drawMessages(gameState.getMessageLog());
        renderer->drawPlayerInfo(gameState.getPlayer(), gameState.getHand());
        renderer->drawField(gameState.getField(),
                           gameState.getAllCharacters(),
                           gameState.getAllBuildings(),
                           gameState.getAllTowers(),
                           gameState.getAllAllies());
    }

    void clearScreen() {
        if (renderer) {
            renderer->clearScreen();
        }
    }

    void renderMessages() {
        if (!game || !renderer) return;
        
        auto& gameState = game->getGameState();
        renderer->drawMessages(gameState.getMessageLog());
    }
};
