#ifndef RENDERER_INTERFACE_HPP
#define RENDERER_INTERFACE_HPP
#include "game_controller.hpp"

class Renderer_interface{
    public:
        virtual ~Renderer_interface() = default;
        virtual void render(const Game_controller& game) = 0;
        virtual void show_help() = 0;
        virtual void show_game_over(const Game_controller& game) = 0;
};

#endif