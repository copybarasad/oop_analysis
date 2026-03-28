#pragma once

#include "Game.h"

class ConsoleRenderer {
public:
    void draw(const Game& game) {
        game.render();
    }
};

