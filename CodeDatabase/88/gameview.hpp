#ifndef GAMEVIEW_HPP
#define GAMEVIEW_HPP

#include "renderer.hpp"
#include "inputhandler.hpp"
#include "../managers/gamemanager.hpp"

template<typename RendererType = ConsoleRenderer>
class GameView {
private:
    RendererType renderer;
    Game* game;
    
public:
    GameView(Game* gameInstance = nullptr) 
        : game(gameInstance) {}
    
    void render();
    void renderMessage(const std::string& message);
    void setGame(Game* gameInstance) {game = gameInstance;}
    RendererType& getRenderer() {return renderer;}
};

#endif