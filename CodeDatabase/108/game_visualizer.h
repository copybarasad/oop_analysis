#ifndef OOP_GAME_VISUALIZER_H
#define OOP_GAME_VISUALIZER_H

#include <memory>
class GameLoop;


template<typename RendererType>
class GameVisualizer {
    std::unique_ptr<RendererType> renderer;
    GameLoop* game;

public:
    explicit GameVisualizer(GameLoop* the_game) : renderer(std::make_unique<RendererType>()), game(the_game) { }

    void render_game() {
        renderer->draw_field(game->get_player(), game->get_game_state(), game->get_current_turn());
    }

};


#endif //OOP_GAME_VISUALIZER_H