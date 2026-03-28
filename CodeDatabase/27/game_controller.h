#pragma once

#include "command.h"
#include "game.h"
#include "i_game_visualizer.h"

template <typename InputProvider>
class GameController {
public:
    GameController(Game& game, IGameVisualizer& visualizer)
        : game(game), visualizer(visualizer) {}

    void Run() {
        visualizer.Render(game);

        while (game.state() != GameState::EXIT) {
            const Command command = input.ReadCommand(game);
            game.Step(command);
            visualizer.Render(game);
        }
    }

private:
    Game& game;
    IGameVisualizer& visualizer;
    InputProvider input;
};
