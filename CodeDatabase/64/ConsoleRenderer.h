#ifndef CONSOLE_RENDERER_H
#define CONSOLE_RENDERER_H

#include "Game.h"

class ConsoleRenderer {
public:
    void render(const Game& game);
    void printMessage(const std::string& message);

private:
    void renderField(const Game& game);
    void renderStats(const Game& game);
    void renderSpells(const Game& game);
};

#endif