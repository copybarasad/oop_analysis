#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include "game_loop.h"

template<typename RendererPolicy, typename InputPolicy>
class GameView {
private:
    GameController& game;
    RendererPolicy renderer;
    GameLoop<InputPolicy, RendererPolicy> loop;

public:
    explicit GameView(GameController& game)
        : game(game), loop(game, renderer) {}

    void run() {
        renderer.render(game);

        while (game.isGameRunning()) {
            if (!loop.runOneTurn()) break;
            renderer.render(game);
        }

        renderer.render(game);

        if (game.isGameWon()) {
            renderer.renderVictory();
        } else {
            renderer.renderGameOver();
        }
    }
};

#endif