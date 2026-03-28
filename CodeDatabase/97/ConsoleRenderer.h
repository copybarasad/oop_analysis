#ifndef UNTITLED_CONSOLERENDERER_H
#define UNTITLED_CONSOLERENDERER_H

#include "GameState.h"

class ConsoleRenderer {
public:
    ConsoleRenderer() = default;
    void draw(const GameState& state);
};

#endif // UNTITLED_CONSOLERENDERER_H
