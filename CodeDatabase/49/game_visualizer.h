#ifndef GAME_VISUALIZER_H
#define GAME_VISUALIZER_H

#include "../game/game.h"

template <typename Renderer>
class GameVisualizer
{
private:
    const Game& game;
    Renderer renderer;

public:
    explicit GameVisualizer(const Game& game_ref);

    void render();
    void clear();
};

template <typename Renderer>
GameVisualizer<Renderer>::GameVisualizer(const Game& game_ref)
    : game(game_ref), renderer() {}

template <typename Renderer>
void GameVisualizer<Renderer>::render()
{
    renderer.render(game);
}

template <typename Renderer>
void GameVisualizer<Renderer>::clear()
{
    renderer.clear_screen();
}

#endif