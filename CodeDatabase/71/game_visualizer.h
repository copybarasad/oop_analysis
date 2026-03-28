#pragma once

#include "game.h"

template <typename RenderPolicy>
class GameVisualizer {
public:
    GameVisualizer(const Game& game) : game_(game) {}

    void render() const {
        if (game_.getField()) {
            renderer_.draw(*game_.getField());
        }
    }

    void displayPlayerTurnInfo() const {
        if (game_.getPlayer() && game_.getField()) {
            renderer_.showPlayerStats(*game_.getPlayer(), *game_.getField());
        }
    }

private:
    const Game& game_; 
    RenderPolicy renderer_; 
};