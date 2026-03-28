#ifndef GAME_DISPLAY_HPP
#define GAME_DISPLAY_HPP
#include <iostream>
#include "game_controller.hpp"
#include "renderer_interface.hpp"

class Game_display: public Renderer_interface{
    private:
        const Game_controller& game_controller;
    public:
        Game_display(const Game_controller& controller);
        void display_game_field() const;
        void display_help() const;
        void display_game_over() const;

        void render(const Game_controller& game) override {display_game_field();}
        void show_help() override {display_help();}
        void show_game_over(const Game_controller& game) override {display_game_over();}
};

#endif