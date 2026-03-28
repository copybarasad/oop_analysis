#pragma once
#include "game.h"

class ConsoleRenderer {
public:
    void render(const Game& game) {
        game.get_field().print_field(
            game.get_player(),
            game.get_enemies()
        );
    }
};
