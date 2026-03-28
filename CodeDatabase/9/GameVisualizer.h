#ifndef GAME_VISUALIZER_H
#define GAME_VISUALIZER_H

#include <SFML/Graphics.hpp>
#include "Game.h"

template <typename Renderer>
class GameVisualizer {
public:
    explicit GameVisualizer(Game& game)
        : game_(game), renderer_(){}

    void RenderFrame(sf::RenderWindow& window) {
        renderer_.Draw(window, game_);
    }

private:
    Game& game_;
    Renderer renderer_;
};

#endif