#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include "game.h"
#include "renderer.h"
#include <memory>

template<typename Renderer_type>
class Game_view {
private:
    Game* game;
    std::unique_ptr<Renderer_type> renderer;

public:
    Game_view(Game* game_instance) 
        : game(game_instance), renderer(std::make_unique<Renderer_type>()) {}
    
    void render() {
        renderer->render(*game);
    }
    
    void display_message(const std::string& message) {
        renderer->display_message(message);
    }
    
    void clear_screen() {
        renderer->clear_screen();
    }
    
    void set_game(Game* game_instance) {
        game = game_instance;
    }
};

#endif