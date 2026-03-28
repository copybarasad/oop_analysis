#ifndef CONSOLE_RENDERER_H
#define CONSOLE_RENDERER_H

#include "game.h"
#include <string>

class ConsoleRenderer {
public:
    void render(const Game& game, const std::string& controls_help);
};

#endif