#pragma once
#include "command.h"

namespace game {
    class Game;

    template<class InputT>
    class GameController {
        InputT input_;

        public:

        void step(Game& game) {
            Command cmd = input_.readCommand(game.state());
            
            game.tick(cmd);
        }

        InputT& input() { return input_; }
    };
}