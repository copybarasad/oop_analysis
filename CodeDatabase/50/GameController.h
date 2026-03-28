#pragma once
#include "CommandReader.h"
#include "Game.h"
#include "IRenderer.h"

template <typename InputHandler>
class GameController {
private:
    Game& game;
    InputHandler& input;

public:
    GameController(Game& g, InputHandler& in)
        : game(g), input(in) {}

    template<typename Renderer>
    void runGame(Renderer& renderer) {
        int flag = 1;
        while (flag) {
            if (!game.runMenuCycle(input, renderer)){
                flag = 0;
            } else {
                game.runLevelCycle(input, renderer);
            }
        }
    }
};
