#ifndef GAME_RENDERER_HPP
#define GAME_RENDERER_HPP
#include "renderer_interface.hpp"

template <typename T_renderer>
class Game_renderer{
    T_renderer renderer;
    public:
        explicit Game_renderer(Game_controller& game): renderer(game){}
        void render(const Game_controller& game){renderer.render(game);}
        void show_help(){renderer.show_help();}
        void show_game_over(const Game_controller& game){renderer.show_game_over(game);}
        Renderer_interface& get_renderer(){return renderer;}
};

#endif